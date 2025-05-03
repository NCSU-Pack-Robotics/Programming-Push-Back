#include "OdometryArc.hpp"

#include "../../subystems/Drivetrain.hpp"
#include "../Utils.hpp"

/**
 * The angular velocity (degrees per second) under which odometry will be calculated using
 * the linear method. Over this value, the arc method will be used.
 */
#define ANGULAR_THRESHOLD 4.0

OdometryArc::OdometryArc(Pose initial_pose)
    : AbstractOdometry(initial_pose) {

    // Begin timing
    this->timer.start();
}

Pose OdometryArc::calculate(const std::pair<double, double> &positions) {
    // Update positions first
    this->left_position = positions.first;
    this->right_position = positions.second;

    // Preliminary calculations
    update_deltas(); // Needs to be done after positions calculations
    update_distance(); // Needs to be done after delta calculations

    // Get elapsed time since last calculation
    const double elapsed_time = this->timer.get_duration();
    this->timer.start();

    /** Angular velocity of the robot using heading (degrees per second) */
    const double angular_velocity = delta_heading / elapsed_time;

    // Update coordinates with the preferred method
    if (fabs(angular_velocity) > ANGULAR_THRESHOLD) {  // If turning
        calculate_position_arc();
    } else {  // If moving somewhat straight
        calculate_position_linear();
    }

    // must be done after coordinate calculations
    this->pose.heading = Constants::Initial::Pose::INITIAL_HEADING +
                         ((right_distance - left_distance) / Constants::Hardware::ROBOT_DIAMETER);

    return this->get_pose();
}

OdometryArc::~OdometryArc() {
    // Implement if needed
}

void OdometryArc::update_deltas() {
    this->delta_left = utils.degrees_to_inches(this->left_position) - left_distance;
    this->delta_right = utils.degrees_to_inches(this->right_position) - right_distance;
    this->delta_avg = (delta_left + delta_right) / 2;
}

void OdometryArc::update_distance() {
    this->left_distance = utils.degrees_to_inches(left_position);
    this->right_distance = utils.degrees_to_inches(right_position);
}

void OdometryArc::calculate_position_linear() {
    this->pose.x += delta_avg * cos(this->pose.heading);
    this->pose.y += delta_avg * sin(this->pose.heading);
}

void OdometryArc::calculate_position_arc() {
    // position calculations
    const double turning_radius = fabs(Constants::Hardware::ROBOT_RADIUS *
                                       (delta_right + delta_left) / (delta_right - delta_left));
    this->delta_heading = (delta_right - delta_left) / Constants::Hardware::ROBOT_DIAMETER;

    // variables to store changes in x andy y
    double delta_x = 0.0;
    double delta_y = 0.0;

    // Calculate the change in x and y
    if ((delta_right - delta_left) != 0) {
        // turning
        if (fabs(delta_right) - fabs(delta_left) > 0) {
            // turning left
            delta_x = turning_radius *
                      (cos(this->pose.heading - Constants::Math::HALF_PI + this->delta_heading ) -
                       cos(this->pose.heading - Constants::Math::HALF_PI));
            delta_y = turning_radius *
                      (sin(this->pose.heading - Constants::Math::HALF_PI + this->delta_heading ) -
                       sin(this->pose.heading - Constants::Math::HALF_PI));
        } else if (fabs(delta_right) - fabs(delta_left) < 0) {
            // turning right
            delta_x = turning_radius *
                      (cos(this->pose.heading + Constants::Math::HALF_PI + this->delta_heading ) -
                       cos(this->pose.heading + Constants::Math::HALF_PI));
            delta_y = turning_radius *
                      (sin(this->pose.heading + Constants::Math::HALF_PI + this->delta_heading ) -
                       sin(this->pose.heading + Constants::Math::HALF_PI));
        }

        // Update x and y coordinates
        this->pose.x += delta_x;
        this->pose.y += delta_y;
    } else {  // Avoids division by zero

        // not turning
        const double delta_average = (delta_left + delta_right) / 2;
        delta_x = delta_average * cos(this->pose.heading);
        delta_y = delta_average * sin(this->pose.heading);
    }
}
