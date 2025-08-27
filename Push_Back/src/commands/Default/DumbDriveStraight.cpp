#include "DumbDriveStraight.hpp"

DumbDriveStraight::DumbDriveStraight(double power, double time) : time(time), power(power) {
}

void DumbDriveStraight::initialize() {
    // Start the timer
    timer.start();

    // Start the drive
    drivetrain.set_drive_power(power, power);
}

void DumbDriveStraight::periodic() {
}

void DumbDriveStraight::shutdown() {
    // Stop the drive
    drivetrain.brake_now();
}

bool DumbDriveStraight::is_complete() {
    return timer.get_duration() >= time;
}
