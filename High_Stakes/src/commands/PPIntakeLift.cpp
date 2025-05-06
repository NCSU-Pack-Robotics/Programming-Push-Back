#include "PPIntakeLift.hpp"

#include "PurePursuit.hpp"

PPIntakeLift::PPIntakeLift(asset path, const double lookahead, const double tolerance): ParallelCommand{} {
    // Add the path to follow
    add_command(std::make_unique<PurePursuit>(path, lookahead, tolerance));

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
    lift.set_braking(true);
    intake.set_braking(true);
}
