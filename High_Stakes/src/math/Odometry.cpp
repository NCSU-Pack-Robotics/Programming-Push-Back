#include "../subystems/Drivetrain.hpp"
#include "Odometry.hpp" // Needs to be after Drivetrain.hpp

Pose Drivetrain::Odometry::odometry(const double new_left_position, const double
new_right_position) {

    return this->get_pose();
}

void Drivetrain::Odometry::update_positions() {

}

void Drivetrain::Odometry::update_deltas() {

}

void Drivetrain::Odometry::update_distance() {

}

void Drivetrain::Odometry::calculate_position_arc() {

}

void Drivetrain::Odometry::calculate_position_linear() {

}

double Drivetrain::Odometry::degrees_to_inches(double position) {
    return 0;
}
