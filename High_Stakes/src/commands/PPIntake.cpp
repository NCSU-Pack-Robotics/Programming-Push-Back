#include "PPIntake.hpp"

#include "PurePursuit.hpp"

PPIntake::PPIntake(asset path, const double lookahead, const double tolerance,
    const double max_speed): ParallelCommand{} {
    // Add the path to follow
    add_command(std::make_unique<PurePursuit>(path, lookahead, tolerance, max_speed));

    // Commands to run lift and intake in parallel
    add_command(std::make_unique<InstantCommand>(std::make_unique<std::function<void()>>(
        [&] { intake.set_drive_power(Constants::Controller::MotorSpeeds::LIFT_UP); })));
}

void PPIntake::initialize() {
}

void PPIntake::shutdown() {
    // Stop intake and lift
    intake.set_drive_power(0);
}
