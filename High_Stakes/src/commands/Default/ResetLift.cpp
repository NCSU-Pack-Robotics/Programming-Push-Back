#include "ResetLift.hpp"

#include "../../Config.hpp"

void ResetLift::initialize() {

}

void ResetLift::periodic() {
// TODO: Figure out if this needs to be in periodic or not
    lift.set_drive_power(Constants::Controller::MotorSpeeds::LIFT_UP);
}

bool ResetLift::is_complete() {
    return hook_sensor.isBlocked();
}


void ResetLift::shutdown() {
    lift.set_drive_power(0);
}


