#include <cmath>

#include "PurePursuit.hpp"

/**
 * @brief function that returns elements in a file line, separated by a delimiter
 *
 * @param input the raw string
 * @param delimiter string separating the elements in the line
 * @return array of elements read from the file
 */
std::vector<std::string> read_element(const std::string &input, const std::string &delimiter) {
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
std::string string_to_hex(const std::string &input) {
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
std::vector<Pose> get_data(const asset &path) {
    std::vector<Pose> robotPath;

    // format data from the asset
    const std::string data(reinterpret_cast<char *>(path.buf), path.size);
    const std::vector<std::string> data_lines = read_element(data, "\n");

    // read the points until 'endData' is read
    for (std::string line: data_lines) {
        // parse line
        if (line == "endData" || line == "endData\r") break;
        const std::vector<std::string> point_input = read_element(line, ", ");

        // check if the line was read correctly
        if (point_input.size() != 3) {
            fprintf(stderr,
                    "Failed to read path file! Are you using the right format? Raw line: {%s}",
                    string_to_hex(line).c_str());
            break;
        }

        Pose path_point(0, 0, 0);
        path_point.x = std::stof(point_input.at(0)); // x position
        path_point.y = std::stof(point_input.at(1)); // y position
        path_point.heading = std::stof(point_input.at(2)); // velocity
        robotPath.push_back(path_point); // save data
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
Pose get_closest_point(const Pose &pose, const std::vector<Pose> &path) {
    Pose closest_point(path[0].x, path[0].y, path[0].heading);
    for (Pose point: path) {
        if (pose.distance(point) < pose.distance(closest_point)) {
            closest_point = point;
        }
    }

    return closest_point;
}

/**
 * Finds the next lookahead point on the path.
 *
 * This is done by first finding the point in the direction of the robot's heading that is the
 * lookahead distance away. Then the point closest to that one on the path is found.
 *
 * @param pose - the current position of the robot
 * @param path - the path to follow
 * @param lookahead_dist - the lookahead distance of the algorithm
 * @return The lookahead point on the path
 */
Pose get_lookahead_point(const Pose &pose, const std::vector<Pose> &path, const double lookahead_dist) {
    // Find the lookahead point in the direction of the robot's heading
    const Pose raw_lookahead(pose.x + lookahead_dist * std::cos(pose.heading),
                            pose.y + lookahead_dist * std::sin(pose.heading), 0);

    // Find the point on the path closest to the lookahead point
    return get_closest_point(raw_lookahead, path);
}

PurePursuit::PurePursuit(const asset &path, const double lookahead, const double tolerance,
    const double max_speed) {
    this->path = get_data(path);
    this->last_point = this->path.back();
    this->lookahead = lookahead;
    this->tolerance = tolerance;
    this->max_speed = max_speed;

    // Ensure path is not empty
    if (this->path.empty()) {
        fprintf(stderr, "No points in path! Do you have the right format? Skipping motion");
        return;
    }

    // Find the dx and dx of the last two points of the path. Will be used later.
    const Pose last_point = this->path.back();
    const Pose second_last_point = this->path[this->path.size() - 2];
    const double dx = last_point.x - second_last_point.x;
    const double dy = last_point.y - second_last_point.y;

    /* Adding some point to the end of the path. When this is not done, the effective lookahead
     * reduces to the distance between the robot and the end of the path. This shrinking of the
     * lookahead causes oscillation towards the end of the path and during shorter paths.
     * This code imply adds more points to the end of the path in the direction of the last point
     * so that there is a points close to the lookahead point when the robot reaches the end of
     * the path. */
    Pose new_last_point = last_point;
    while (last_point.distance(new_last_point) < lookahead * 2) {  // *2 just to be safe
        new_last_point = Pose(new_last_point.x + dx, new_last_point.y + dy, new_last_point.heading);
        this->path.push_back(new_last_point);
    }
}

void PurePursuit::initialize() {
    done = false;
    drivetrain.set_braking(false);
}

void PurePursuit::periodic() {
    const Pose pose = drivetrain.get_pose();

    const Pose lookahead_point = get_lookahead_point(pose, path, lookahead);

    // Get variables to make equation less cluttered
    const double m = tan(fmod(pose.heading, M_PI / 2) == 0 ? pose.heading + 1e-9 : pose.heading);  // slope of the robot
    // const double m = tan(pose.heading);  // slope of the robot
    const double xl = lookahead_point.x; // x of the lookahead point
    const double yl = lookahead_point.y; // y of the lookahead point
    const double x = pose.x; // x of the robot
    const double y = pose.y; // y of the robot

    // calculate radius of the circle connecting the robot and the lookahead point
    const double radius = (sqrt(pow(m, 2) + 1) * (pow(xl - x, 2) + pow(yl - y, 2)))
                          / (2 * fabs(m * (xl - x) - (yl - y)));

    // curvature of the circle connecting the robot and the lookahead point
    double curvature = 1 / radius;

    // Get target velocity from the path
    const double target_vel = get_closest_point(pose, path).heading;

    // Add sign to curvature based on the side of the robot the lookahead point is on
    const double dx = lookahead_point.x - pose.x;
    const double dy = lookahead_point.y - pose.y;
    const double side = dx * sin(pose.heading) - dy * cos(pose.heading);
    curvature *= side;

    // calculate target left and right velocities
    double target_left_vel = target_vel * (2 + curvature * (Constants::Hardware::ROBOT_DIAMETER)) / 2;
    double target_right_vel = target_vel * (2 - curvature * (Constants::Hardware::ROBOT_DIAMETER)) / 2;

    // ratio the speeds to respect the max speed
    const double ratio = std::max(std::fabs(target_left_vel), std::fabs(target_right_vel)) / this->max_speed;
    if (ratio > 1) {
        target_left_vel /= ratio;
        target_right_vel /= ratio;
    }

    // Set power  TODO: Maybe make this voltage
    drivetrain.set_drive_power(static_cast<int>(target_left_vel), static_cast<int>(target_right_vel));

    // Debugging
    // printf("Curvature: %9f | Velocity: %9f | Motors: %4.0f %4.0f| Pose: %50s | Target X: %9f Y: %9f | Distance: %f\n",
    //     curvature, target_vel, target_left_vel, target_right_vel, pose.to_string().c_str(),
    //     lookahead_point.x, lookahead_point.y, pose.distance(path.back()));

    // Printing for graph via csv
    // printf("%f, %f, %f, %f\n", pose.x, pose.y, lookahead_point.x, lookahead_point.y);
}

void PurePursuit::shutdown() {
    // stop the robot
    drivetrain.brake_now();
}

bool PurePursuit::is_complete() {
    const Pose pose = drivetrain.get_pose();

    // When the robot is within some inches of the end of the path, stop
    if (pose.distance(this->last_point) < this->tolerance) {
        return true;
    }

    return false;
}

std::string PurePursuit::to_string() const {
    std::string result = "PurePursuit(";
    
    result += std::to_string(path.size()) + " points, ";
    
    Pose current = drivetrain.get_pose();
    
    if (!path.empty()) {
        Pose closest = get_closest_point(current, path);

        size_t pointsLeft = 0;
        // Find the point closest to the robot and count how many points are left
        for (size_t i = 0; i < path.size(); i++) {
            if (path[i].x == closest.x && path[i].y == closest.y && path[i].heading == closest.heading) {
                pointsLeft = path.size() - i;
                break;
            }
        }
        result += std::to_string(pointsLeft) + " points left, ";
        result += "start: (" + std::to_string(path[0].x) + ", " + std::to_string(path[0].y) + "), ";
        result += "end: (" + std::to_string(last_point.x) + ", " + std::to_string(last_point.y) + "), ";
        
        result += "current: (" + std::to_string(current.x) + ", " + std::to_string(current.y) + "), ";
        Pose next = get_lookahead_point(current, path, lookahead);
        result += "next: (" + std::to_string(next.x) + ", " + std::to_string(next.y) + "), ";
    }
    
    result += "lookahead: " + std::to_string(lookahead) + ", ";
    result += "tolerance: " + std::to_string(tolerance) + ", ";
    result += "max_speed: " + std::to_string(max_speed) + ")";
    
    return result;
}