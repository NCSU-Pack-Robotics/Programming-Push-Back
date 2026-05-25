#include "Turn.hpp"

void Turn::initialize() {
    this->done = false;
    this->initial_heading = drivetrain.get_pose().heading;
}

void Turn::periodic() {
    // If the command is done, do nothing
    if (this->done) return;

    // Update the distance driven
    const double degrees_turned = drivetrain.get_pose().heading - this->initial_heading;
    const double error = this->target_degrees - degrees_turned;
    const double output = this->pid.calculate(error);

    // Move drivetrain
    // drivetrain.set_voltage(-output, output);

    // Check if we are done
    if (fabs(error) < this->tolerance) {
        this->done = true;
    }
}

void Turn::shutdown() {
    drivetrain.brake_now();
}

bool Turn::is_complete() {
    return this->done;
}
