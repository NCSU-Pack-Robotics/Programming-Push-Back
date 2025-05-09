#include "PPIntakeLift.hpp"

#include "PurePursuit.hpp"

PPIntakeLift::PPIntakeLift(asset path, const double lookahead, const double tolerance,
    const double max_speed): ParallelCommand{} {
    // Add the path to follow
    add_command(std::make_unique<PurePursuit>(path, lookahead, tolerance, max_speed));

    // Commands to run lift and intake in parallel
    add_command(std::make_unique<InstantCommand>(std::make_unique<std::function<void()>>(
        [&] { lift.set_drive_power(Constants::Controller::MotorSpeeds::LIFT_UP); })));
    add_command(std::make_unique<InstantCommand>(std::make_unique<std::function<void()>>(
        [&] { intake.set_drive_power(Constants::Controller::MotorSpeeds::INTAKE_INWARDS); })));
}

void PPIntakeLift::initialize() {
}

void PPIntakeLift::shutdown() {
    // Stop intake and lift
    lift.set_drive_power(0);
    intake.set_drive_power(0);

    // Stop the drivetrain
    drivetrain.brake_now();
}
