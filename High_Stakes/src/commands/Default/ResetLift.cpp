#include "ResetLift.hpp"

#include "../../Config.hpp"

void ResetLift::initialize() {
    has_passed = false;
}

void ResetLift::periodic() {
    
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
    // TODO: Figure out why shutdown is never called ?!?
}



