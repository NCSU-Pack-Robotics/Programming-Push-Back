#include "DriveStraight.hpp"

void DriveForward::initialize() {
    this->done = false;
    this->distance_driven = 0;
}

void DriveForward::periodic() {
    // If the command is done, do nothing
    if (this->done) return;

    // Update the distance driven
    const double error = this->target_distance - this->distance_driven;
    const double output = this->pid.calculate(error);

    // Move drivetrain
    this->drivetrain.set_voltage(output, output);

    // Check if we are done
    if (fabs(error) < this->tolerance) {
        this->done = true;
    }
}

void DriveForward::shutdown() {
    drivetrain.brake();
}

bool DriveForward::is_complete() {
    return this->done;
}
