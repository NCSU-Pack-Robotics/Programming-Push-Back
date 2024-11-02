#include "../subystems/Drivetrain.hpp"
#include "Odometry.hpp" // Needs to be after Drivetrain.hpp
#include "../Constants.hpp"

Pose Drivetrain::Odometry::calculate(const double new_left_position,
                                    const double new_right_position) {
    // Preliminary calculations
    update_positions();
    update_deltas();  // Needs to be done after positions calculations
    update_distance();  // Needs to be done after delta calculations

    // Update coordinates with preferred method
    calculate_position_arc();

    // must be done after coordinate calculations
    this->pose.heading = (right_distance - left_distance) / (Constants::Hardware::ROBOT_DIAMETER);

    return this->get_pose();
}

Pose Drivetrain::Odometry::get_pose() const {
    return Pose();
}

void Drivetrain::Odometry::calculate_position_arc() {
    // position calculations
    double turning_radius = fabs(Constants::Hardware::ROBOT_RADIUS * (delta_right+delta_left)
            / (delta_right-delta_left));
    double delta_heading = (delta_right-delta_left) / (Constants::Hardware::ROBOT_DIAMETER);

    // variables to store changes in x andy y
    double delta_x = 0.0;
    double delta_y = 0.0;

    // Calculate the change in x and y
    if ((delta_right - delta_left) != 0) { // turning
        if (fabs(delta_right)-fabs(delta_left) > 0) { // turning left
            delta_x = turning_radius *
                    (cos(this->pose.heading - Constants::Math::HALF_PI + delta_heading)
                    - cos(this->pose.heading - Constants::Math::HALF_PI));
            delta_y = turning_radius *
                    (sin(this->pose.heading - Constants::Math::HALF_PI + delta_heading)
                    - sin(this->pose.heading - Constants::Math::HALF_PI));

        } else if (fabs(delta_right)-fabs(delta_left) < 0) { // turning right
            delta_x = turning_radius *
                    (cos(this->pose.heading + Constants::Math::HALF_PI + delta_heading)
                    - cos(this->pose.heading + Constants::Math::HALF_PI));
            delta_y = turning_radius *
                    (sin(this->pose.heading + Constants::Math::HALF_PI + delta_heading)
                    - sin(this->pose.heading + Constants::Math::HALF_PI));
        }

    } else { // not turning
        double delta_average = (delta_left + delta_right) / 2;
        delta_x = delta_average * cos(this->pose.heading);
        delta_y = delta_average * sin(this->pose.heading);
    }

    // Update x and y coords
    this->pose.x += delta_x;
    this->pose.y += delta_y;
}

void Drivetrain::Odometry::calculate_position_linear() {
    this->pose.x += delta_avg * cos(this->pose.heading);
    this->pose.y += delta_avg * sin(this->pose.heading);
}

void Drivetrain::Odometry::update_positions() {
    this->left_position = drivetrain.get_position().first;
    this->right_position = drivetrain.get_position().second;
}

void Drivetrain::Odometry::update_deltas() {
    this->delta_left = degrees_to_inches(this->left_position) - left_distance;
    this->delta_right = degrees_to_inches(this->right_position) - right_distance;
    this->delta_avg = (delta_left + delta_right) / 2;
}

void Drivetrain::Odometry::update_distance() {
    this->left_distance = degrees_to_inches(left_position);
    this->right_distance = degrees_to_inches(right_position);
}

double Drivetrain::Odometry::degrees_to_inches(const double position) {
    return Constants::Hardware::TRACKING_RATIO * Constants::Hardware::TRACKING_DIAMETER * Constants::Math::PI * (position / 360);
}
