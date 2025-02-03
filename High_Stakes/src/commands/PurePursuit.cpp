#include "PurePursuit.hpp"

#include <cmath>

/**
 * @brief function that returns elements in a file line, separated by a delimiter
 *
 * @param input the raw string
 * @param delimiter string separating the elements in the line
 * @return array of elements read from the file
 */
std::vector<std::string> readElement(const std::string &input, const std::string &delimiter) {
    std::string s = input;
    std::vector<std::string> output;
    size_t pos = 0;

    // main loop
    while ((pos = s.find(delimiter)) != std::string::npos) {
        // while there are still delimiters in the string
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
std::string stringToHex(const std::string &input) {
    static constexpr char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (const unsigned char c: input) {
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
std::vector<Pose> getData(const asset &path) {
    std::vector<Pose> robotPath;

    // format data from the asset
    const std::string data(reinterpret_cast<char *>(path.buf), path.size);
    const std::vector<std::string> dataLines = readElement(data, "\n");

    // read the points until 'endData' is read
    for (std::string line: dataLines) {
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
 * Finds the closest point on the path to the robot.
 *
 * @param pose - the current position of the robot
 * @param path - the path to follow
 * @return The closest point on the path to the robot
 */
Pose getClosestPoint(const Pose &pose, const std::vector<Pose> &path) {
    Pose closestPoint(path[0].x, path[0].y, path[0].heading);
    for (Pose point: path) {
        if (pose.distance(point) < pose.distance(closestPoint)) {
            closestPoint = point;
        }
    }

    return closestPoint;
}

/**
 * Finds the next lookahead point on the path.
 *
 * This is done by first finding the point in the direction of the robot's heading that is the
 * lookahead distance away. Then the point closest to that one on the path is found.
 *
 * @param pose - the current position of the robot
 * @param path - the path to follow
 * @param lookaheadDist - the lookahead distance of the algorithm
 * @return The lookahead point on the path
 */
Pose getLookaheadPoint(const Pose &pose, const std::vector<Pose> &path, const double lookaheadDist) {
    // Find the lookahead point in the direction of the robot's heading
    const Pose rawLookahead(pose.x + lookaheadDist * std::cos(pose.heading),
                            pose.y + lookaheadDist * std::sin(pose.heading), 0);

    // Find the point on the path closest to the lookahead point
    return getClosestPoint(rawLookahead, path);
}

PurePursuit::PurePursuit(const asset &path) : path(getData(path)) {
    if (this->path.size() == 0) {
        fprintf(stderr, "No points in path! Do you have the right format? Skipping motion");
        return;
    }
}

void PurePursuit::initialize() {
}

void PurePursuit::periodic() {
    const Pose pose = drivetrain.get_pose();

    // When the robot is within some inches of the end of the path, stop
    if (pose.distance(path.back()) < Constants::PathFollowing::STOP_DISTANCE) {
        done = true;
        return;
    }

    const Pose lookaheadPoint = getLookaheadPoint(pose, path, lookahead);

    // Get variables to make equation less cluttered
    const double m = tan(fmod(pose.heading, M_PI / 2) == 0 ? pose.heading + 1e-9 : pose.heading);  // slope of the robot
    // const double m = tan(pose.heading);  // slope of the robot
    const double xl = lookaheadPoint.x; // x of the lookahead point
    const double yl = lookaheadPoint.y; // y of the lookahead point
    const double x = pose.x; // x of the robot
    const double y = pose.y; // y of the robot

    // calculate radius of the circle connecting the robot and the lookahead point
    const double radius = (sqrt(pow(m, 2) + 1) * (pow(xl - x, 2) + pow(yl - y, 2)))
                          / (2 * fabs(m * (xl - x) - (yl - y)));

    // curvature of the circle connecting the robot and the lookahead point
    double curvature = 1 / radius;

    // Get target velocity from the path
    const double targetVel = getClosestPoint(pose, path).heading;

    // Add sign to curvature based on the side of the robot the lookahead point is on
    const double dTheta = atan2(lookaheadPoint.y - pose.y, lookaheadPoint.x - pose.x) - pose.heading;
    const double side = dTheta > 0 ? -1 : 1;  // -1 for left, 1 for right
    curvature *= side;

    // calculate target left and right velocities
    double targetLeftVel = targetVel * (2 + curvature * (Constants::Hardware::ROBOT_DIAMETER)) / 2;
    double targetRightVel = targetVel * (2 - curvature * (Constants::Hardware::ROBOT_DIAMETER)) / 2;

    // ratio the speeds to respect the max speed
    const double ratio = std::max(std::fabs(targetLeftVel), std::fabs(targetRightVel)) / 80;
    if (ratio > 1) {
        targetLeftVel /= ratio;
        targetRightVel /= ratio;
    }

    drivetrain.set_drive_power(targetLeftVel, targetRightVel);

    // Debugging
    // printf("Last Point: %s\n", path.back().to_string().c_str());
    printf("Curvature: %9f | Velocity: %9f | Motors: %4.0f %4.0f| Pose: %50s | Target X: %9f Y: %9f | Distance: %f\n | Done: %d\n",
        curvature, targetVel, targetLeftVel, targetRightVel, pose.to_string().c_str(), lookaheadPoint.x, lookaheadPoint.y, pose.distance(path.back()), done);
}

void PurePursuit::shutdown() {
    printf("DONE!\n");

    // stop the robot
    drivetrain.stop();
}

bool PurePursuit::is_complete() {
    return this->done;
}
