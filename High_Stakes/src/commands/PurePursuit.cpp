#include "PurePursuit.hpp"

#include <cmath>
/**
 * @brief function that returns elements in a file line, separated by a delimiter
 *
 * @param input the raw string
 * @param delimiter string separating the elements in the line
 * @return array of elements read from the file
 */
std::vector<std::string> readElement(const std::string& input, const std::string& delimiter) {
    std::string s = input;
    std::vector<std::string> output;
    size_t pos = 0;

    // main loop
    while ((pos = s.find(delimiter)) != std::string::npos) { // while there are still delimiters in the string
        std::string token = s.substr(0, pos); // processed substring
        output.push_back(token);
        s.erase(0, pos + delimiter.length()); // remove the read substring
    }

    output.push_back(s); // add the last element to the returned string

    return output;
}

/**
 * @brief Convert a string to hex
 *
 * @param input the string to convert
 * @return std::string hexadecimal output
 */
std::string stringToHex(const std::string& input) {
    static constexpr char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (const unsigned char c : input) {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

/**
 * @brief Get a path from the asset
 *
 * @param path The asset containing the path to follow
 * @return vector of pose points on the path
 */
std::vector<Pose> getData(const asset& path) {
    std::vector<Pose> robotPath;

    // format data from the asset
    const std::string data(reinterpret_cast<char*>(path.buf), path.size);
    const std::vector<std::string> dataLines = readElement(data, "\n");

    // read the points until 'endData' is read
    for (std::string line : dataLines) {
        // parse line
        if (line == "endData" || line == "endData\r") break;
        const std::vector<std::string> pointInput = readElement(line, ", ");

        // check if the line was read correctly
        if (pointInput.size() != 3) {
            fprintf(stderr,
                "Failed to read path file! Are you using the right format? Raw line: {%s}",
                stringToHex(line).c_str());
            break;
        }

        Pose pathPoint(0, 0, 0);
        pathPoint.x = std::stof(pointInput.at(0)); // x position
        pathPoint.y = std::stof(pointInput.at(1)); // y position
        pathPoint.heading = std::stof(pointInput.at(2)); // velocity
        robotPath.push_back(pathPoint); // save data
    }

    return robotPath;
}

/**
 * @brief find the closest point on the path to the robot
 *
 * @param pose the current pose of the robot
 * @param path the path to follow
 * @return int index to the closest point
 */
int findClosest(const Pose& pose, const std::vector<Pose>& path) {
    int closestPoint = 0;
    float closestDist = std::numeric_limits<float>::infinity();

    // loop through all path points
    for (int i = 0; i < path.size(); i++) {
        if (const float dist = pose.distance(path.at(i)); dist < closestDist) {
            // new closest point
            closestDist = dist;
            closestPoint = i;
        }
    }

    return closestPoint;
}

/**
 * @brief Function that finds the intersection point between a circle and a line
 *
 * @param p1 start point of the line
 * @param p2 end point of the line
 * @param pose position of the robot
 * @param lookaheadDist the lookahead distance of the algorithm
 * @return float how far along the line the
 */
float circleIntersect(const Pose& p1, const Pose& p2, const Pose& pose, const float lookaheadDist) {
    // calculations
    // uses the quadratic formula to calculate intersection points
    const Pose d = p2 - p1;
    const Pose f = p1 - pose;
    const float a = d * d;
    const float b = 2 * (f * d);
    const float c = (f * f) - lookaheadDist * lookaheadDist;
    float discriminant = b * b - 4 * a * c;

    // if a possible intersection was found
    if (discriminant >= 0) {
        discriminant = sqrt(discriminant);
        const float t1 = (-b - discriminant) / (2 * a);
        const float t2 = (-b + discriminant) / (2 * a);

        // prioritize further down the path
        if (t2 >= 0 && t2 <= 1) return t2;
        if (t1 >= 0 && t1 <= 1) return t1;
    }

    // no intersection found
    return -1;
}

/**
 * @brief returns the lookahead point
 *
 * @param lastLookahead - the last lookahead point
 * @param pose - the current position of the robot
 * @param path - the path to follow
 * @param closest - the index of the point closest to the robot
 * @param lookaheadDist - the lookahead distance of the algorithm
 */
Pose lookaheadPoint(const Pose& lastLookahead, const Pose& pose, const std::vector<Pose>& path,
    const int closest, const float lookaheadDist) {
    // optimizations applied:
    // only consider intersections that have an index greater than or equal to the point closest
    // to the robot
    // and intersections that have an index greater than or equal to the index of the last
    // lookahead point
    const int start = std::max(closest, static_cast<int>(lastLookahead.heading));
    for (int i = start; i < path.size() - 1; i++) {
        Pose lastPathPose = path.at(i);
        Pose currentPathPose = path.at(i + 1);

        float t = circleIntersect(lastPathPose, currentPathPose, pose, lookaheadDist);

        if (t != -1) {
            Pose lookahead = lastPathPose.lerp(currentPathPose, t);
            lookahead.heading = i;
            return lookahead;
        }
    }

    // robot deviated from path, use last lookahead point
    return lastLookahead;
}

/**
 * @brief Get the curvature of a circle that intersects the robot and the lookahead point
 *
 * @param pose the position of the robot
 * @param heading the heading of the robot
 * @param lookahead the lookahead point
 * @return float curvature
 */
float findLookaheadCurvature(const Pose& pose, const float heading, const Pose& lookahead) {
    // calculate whether the robot is on the left or right side of the circle
    double side = std::sin(heading) * (lookahead.x - pose.x) - std::cos(heading) * (lookahead.y - pose.y);
    side = side > 0 ? 1 : -1;

    // calculate center point and radius
    const float a = -std::tan(heading);
    const float c = std::tan(heading) * pose.x - pose.y;
    const float x = std::fabs(a * lookahead.x + lookahead.y + c) / std::sqrt((a * a) + 1);
    const float d = std::hypot(lookahead.x - pose.x, lookahead.y - pose.y);

    // return curvature
    return side * ((2 * x) / (d * d));
}

/**
 * @brief Slew a value to a target.
 *
 * Slew is used to limit the rate of change of a value. This is useful for limiting the acceleration
 * or jerkiness of a system.
 *
 * @param target the target value
 * @param current the current value
 * @param maxChange the maximum change allowed
 * @return float the slewed value
 */
float slew(const float target, const float current, const float maxChange) {
    float change = target - current;
    if (maxChange == 0) return target;
    if (change > maxChange) change = maxChange;
    else if (change < -maxChange) change = -maxChange;
    return current + change;
}

PurePursuit::PurePursuit(const asset& path) : path(getData(path)) {
    if (this->path.size() == 0) {
        fprintf(stderr, "No points in path! Do you have the right format? Skipping motion");
        return;
    }
}

void PurePursuit::initialize() {
    current = this->drivetrain.get_pose();

    lastPose = current;
    lookaheadPose = path[0];
    lastLookahead = lookaheadPose;
    lookaheadPose.heading = 0;
}

void PurePursuit::periodic() {
    current = drivetrain.get_pose();

    distTraveled += current.distance(lastPose);
    lastPose = current;

    // find the closest point on the path to the robot
    const float closestPoint = findClosest(current, path);
    // if the robot is at the end of the path, then stop
    if (path.at(closestPoint).heading == 0) {
        done = true;
        return;
    }

    // find the lookahead point
    lookaheadPose = lookaheadPoint(lastLookahead, current, path, closestPoint, lookahead);
    lastLookahead = lookaheadPose; // update last lookahead position

    // get the curvature of the arc between the robot and the lookahead point
    const float curvatureHeading = M_PI / 2 - current.heading;
    const float curvature = findLookaheadCurvature(current, curvatureHeading, lookaheadPose);

    // get the target velocity of the robot
    float targetVel = path.at(closestPoint).heading;
    targetVel = slew(targetVel, prevVelocity, 20);  // TODO: maxChange adjustment
    prevVelocity = targetVel;

    // calculate target left and right velocities
    float targetLeftVel = targetVel * (2 + curvature * Constants::Hardware::ROBOT_DIAMETER) / 2;
    float targetRightVel = targetVel * (2 - curvature * Constants::Hardware::ROBOT_DIAMETER) / 2;

    // ratio the speeds to respect the max speed
    const float ratio = std::max(std::fabs(targetLeftVel), std::fabs(targetRightVel)) / 127;
    if (ratio > 1) {
        targetLeftVel /= ratio;
        targetRightVel /= ratio;
    }

    // update previous velocities
    prevLeftVal = targetLeftVel;
    prevRightVal = targetRightVel;

    drivetrain.set_drive_power(targetLeftVel, targetRightVel);
}

void PurePursuit::shutdown() {
    // stop the robot
    drivetrain.set_drive_power(0, 0);
}

bool PurePursuit::is_complete() {
    return this->done;
}
