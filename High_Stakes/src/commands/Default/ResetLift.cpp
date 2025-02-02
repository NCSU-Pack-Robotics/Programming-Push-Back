#include "ResetLift.hpp"

#include "../../Config.hpp"

void ResetLift::initialize() {
    has_passed = false;
}

void ResetLift::periodic() {
    if (hook_sensor.isBlocked()) {
        has_passed = true;
        ticks_since_passed = 0;
    } else if (!has_passed){
        lift.set_drive_power(Constants::Controller::MotorSpeeds::LIFT_RESET);
    }

    if (has_passed) {
        ticks_since_passed++;
        lift.set_drive_power(-Constants::Controller::MotorSpeeds::LIFT_RESET);
    }

}

bool ResetLift::is_complete() {
    // Temporary fix because shutdown is not being called
    // This delay of 90 is assuming the opcontrol loop runs with a 1 MS delay.
    if (has_passed && ticks_since_passed >= 90) {
        lift.brake();
        return true;
    }
    return false;
}


void ResetLift::shutdown() {
    // TODO: Figure out why shutdown is never called ?!?
}


