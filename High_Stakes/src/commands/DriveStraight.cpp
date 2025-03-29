#include "DriveStraight.hpp"

void DriveStraight::initialize() {
    this->done = false;

    // Get the initial distances
    this->initial_left_distance = Drivetrain::degrees_to_inches(drivetrain.get_position().first);
    this->initial_right_distance = Drivetrain::degrees_to_inches(drivetrain.get_position().second);
}

void DriveStraight::periodic() {
    // If the command is done, do nothing
    if (this->done) return;

    // Update the distances drive
    const double distance_driven_left = Drivetrain::degrees_to_inches(drivetrain.get_position().first);
    const double distance_driven_right = Drivetrain::degrees_to_inches(drivetrain.get_position().second);

    // Get errors
    const double left_error = target_distance - distance_driven_left;
    const double right_error = target_distance - distance_driven_right;

    // Calculate new voltages
    int32_t left_voltage = static_cast<int32_t>(pid.calculate(left_error));
    int32_t right_voltage = static_cast<int32_t>(pid.calculate(right_error));

    // Ensure not too fast
    left_voltage = std::clamp<int32_t>(left_voltage, -8000, 8000);
    right_voltage = std::clamp<int32_t>(right_voltage, -8000, 8000);

    // Set the new voltages
    drivetrain.set_voltage(left_voltage, right_voltage);

    // Check if the command is done
    if (std::abs(target_distance - distance_driven_left) < tolerance &&
        std::abs(target_distance - distance_driven_right) < tolerance) {

        this->done = true;
        // drivetrain.brake_now();
    }
}

void DriveStraight::shutdown() {
    // drivetrain.brake_now();
}

bool DriveStraight::is_complete() {
    return this->done;
}
