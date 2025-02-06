#include "ResetLift.hpp"

#include "../../Config.hpp"

void ResetLift::initialize() {
    has_passed = false;
}

void ResetLift::periodic() {
    
    // Do not activate if LadyBrown is in stow position.
    if (lady_brown.get_position() == LadyBrown::Position::STOW) return;

    if (hook_sensor.isBlocked()) {
        lift.brake();
        has_passed = true;
    } else {
        lift.set_drive_power(Constants::Controller::MotorSpeeds::LIFT_RESET);
    };

}

bool ResetLift::is_complete() {
    return has_passed;
}


void ResetLift::shutdown() {

}



