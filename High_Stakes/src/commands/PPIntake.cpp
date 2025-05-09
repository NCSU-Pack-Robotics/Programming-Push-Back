#include "PPIntake.hpp"

#include "PurePursuit.hpp"

PPIntake::PPIntake(asset path, const double lookahead, const double tolerance, const double max_speed, const double intake_speed) :
        PurePursuit(path, lookahead, tolerance, max_speed){
    this->intake_speed = intake_speed;
}

void PPIntake::initialize() {
    intake.set_drive_power(this->intake_speed);
}

void PPIntake::shutdown() {
    PurePursuit::shutdown();

    // Stop intake and lift
    intake.set_drive_power(0);

    drivetrain.brake_now();
}
