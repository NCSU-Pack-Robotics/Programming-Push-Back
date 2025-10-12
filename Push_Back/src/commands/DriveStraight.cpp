#include "DriveStraight.hpp"

#include "../math/Utils.hpp"

void DriveStraight::initialize() {
    this->done = false;

    // Get the initial distances
    this->initial_left_distance = Utils::degrees_to_inches(drivetrain.get_position().first);
    this->initial_right_distance = Utils::degrees_to_inches(drivetrain.get_position().second);

    /** Begin the timer */
    timer.start();
}

void DriveStraight::periodic() {
    // If maximum time for the command has been reached, stop the command
    if (timer.get_duration() > max_time) {
        this->done = true;
    }

    // If the command is done, do nothing
    if (this->done) return;

    // Get the absolute motor positions
    double distance_driven_left = Utils::degrees_to_inches(drivetrain.get_position(true).first);
    double distance_driven_right = Utils::degrees_to_inches(drivetrain.get_position(true).second);

    // Calculate the distance driven since the start of the command
    distance_driven_left -= this->initial_left_distance;
    distance_driven_right -= this->initial_right_distance;
    distance_driven = (distance_driven_right + distance_driven_left) / 2;

    // Get errors
    const double left_error = target_distance - distance_driven_left;
    const double right_error = target_distance - distance_driven_right;

    // Calculate new voltages
    auto left_voltage = static_cast<int32_t>(pid_left.calculate(left_error));
    auto right_voltage = static_cast<int32_t>(pid_right.calculate(right_error));

    // Ensure not too fast
    left_voltage = std::clamp<int32_t>(left_voltage, -8000, 8000);
    right_voltage = std::clamp<int32_t>(right_voltage, -8000, 8000);

    // Set the new voltages
    drivetrain.set_voltage(left_voltage, right_voltage);

    // Check if the command is done
    if (std::abs(target_distance - distance_driven_right) < tolerance) {
        this->done = true;
    }
}

void DriveStraight::shutdown() {
    drivetrain.brake_now();
}

bool DriveStraight::is_complete() {
    return this->done;
}

double DriveStraight::get_progress() {
    return std::clamp(distance_driven / target_distance, 0.0, 1.0);
}
