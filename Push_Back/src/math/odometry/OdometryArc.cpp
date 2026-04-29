#include "OdometryArc.hpp"

#include "../../subsystems/Drivetrain.hpp"
#include "../Utils.hpp"

/**
 * The angular velocity (degrees per second) under which odometry will be calculated using
 * the linear method. Over this value, the arc method will be used.
 */
#define ANGULAR_THRESHOLD 1000000000

OdometryArc::OdometryArc(const Pose& initial_pose)
    : AbstractOdometry(initial_pose) {

    // Begin timing
    this->timer.start();
}

Pose OdometryArc::calculate(const std::pair<double, double> &positions) {
    // Update positions first
    this->left_position = positions.first - this->reset_position_left;
    this->right_position = positions.second - this->reset_position_right;

    // Preliminary calculations
    update_deltas(); // Needs to be done after positions calculations
    update_distance(); // Needs to be done after delta calculations

    // Get elapsed time since last calculation
    const double elapsed_time = this->timer.get_duration();
    this->timer.start();

    /** Angular velocity of the robot using heading (degrees per second) */
    const double angular_velocity = delta_heading / elapsed_time;

    // Update coordinates with the preferred method
    if (fabs(angular_velocity) > ANGULAR_THRESHOLD) { // If turning
        calculate_position_arc();
    } else { // If moving somewhat straight
        calculate_position_linear();
    }

    // must be done after coordinate calculations
    update_heading();

    return this->get_pose();
}

void OdometryArc::set_pose(const Pose new_pose) {
    AbstractOdometry::set_pose(new_pose);
    this->pose = new_pose;

    this->pose_updated = true;

    // Update the last reset positions
    this->reset_position_left = this->left_position;
    this->reset_position_right = this->right_position;
    this->reset_heading = new_pose.heading;

    // Reset fields
    this->left_position = 0;
    this->right_position = 0;
    this->left_distance = 0;
    this->right_distance = 0;
    this->delta_left = 0;
    this->delta_right = 0;
    this->delta_avg = 0;
    this->delta_heading = 0;
    this->timer.start();
}

OdometryArc::~OdometryArc() {
    // Implement if needed
}

void OdometryArc::update_deltas() {
    this->delta_left = Utils::degrees_to_inches(this->left_position) - left_distance;
    this->delta_right = Utils::degrees_to_inches(this->right_position) - right_distance;
    this->delta_avg = (delta_left + delta_right) / 2;
    this->delta_heading = (delta_right - delta_left) / Constants::Hardware::ROBOT_DIAMETER;
}

void OdometryArc::update_distance() {
    this->left_distance = Utils::degrees_to_inches(left_position);
    this->right_distance = Utils::degrees_to_inches(right_position);
}

void OdometryArc::update_heading() {
    const double initial_heading = this->pose_updated ? this->reset_heading : Constants::Initial::Pose::INITIAL_HEADING;
    this->pose.heading = initial_heading +
                         ((right_distance - left_distance) / Constants::Hardware::ROBOT_DIAMETER);
}

void OdometryArc::calculate_position_linear() {
    this->pose.x += delta_avg * cos(this->pose.heading);
    this->pose.y += delta_avg * sin(this->pose.heading);
}

void OdometryArc::calculate_position_arc() {
    // position calculations
    const double turning_radius = fabs(Constants::Hardware::ROBOT_RADIUS *
                                       (delta_right + delta_left) / (delta_right - delta_left));

    // variables to store changes in x andy y
    double delta_x = 0.0;
    double delta_y = 0.0;

    // Calculate the change in x and y
    if ((delta_right - delta_left) != 0) {
        // turning
        if (fabs(delta_right) - fabs(delta_left) > 0) {
            // turning left
            delta_x = turning_radius *
                      (cos(this->pose.heading - M_PI_2 + this->delta_heading ) -
                       cos(this->pose.heading - M_PI_2));
            delta_y = turning_radius *
                      (sin(this->pose.heading - M_PI_2 + this->delta_heading ) -
                       sin(this->pose.heading - M_PI_2));
        } else if (fabs(delta_right) - fabs(delta_left) < 0) {
            // turning right
            delta_x = turning_radius *
                      (cos(this->pose.heading + M_PI_2 + this->delta_heading ) -
                       cos(this->pose.heading + M_PI_2));
            delta_y = turning_radius *
                      (sin(this->pose.heading + M_PI_2 + this->delta_heading ) -
                       sin(this->pose.heading + M_PI_2));
        }
    } else {  // Avoids division by zero

        // not turning
        const double delta_average = (delta_left + delta_right) / 2;
        delta_x = delta_average * cos(this->pose.heading);
        delta_y = delta_average * sin(this->pose.heading);
    }

    // Update the pose's x and y coordinates
    this->pose.x += delta_x;
    this->pose.y += delta_y;
}
