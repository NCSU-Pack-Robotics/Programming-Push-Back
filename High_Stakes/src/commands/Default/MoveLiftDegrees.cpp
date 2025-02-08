#include "MoveLiftDegrees.hpp"

#include "../../subystems/Lift.hpp"
#include "Config.hpp"

MoveLiftDegrees::MoveLiftDegrees(int32_t degrees) {
    start_degree = lift.get_degrees();
    target_degree = start_degree + degrees;
}

void MoveLiftDegrees::initialize() {
    int32_t speed;
    if (target_degree > start_degree) {
        speed = Constants::Controller::MotorSpeeds::LIFT_UP;
    } else {
        speed = Constants::Controller::MotorSpeeds::LIFT_DOWN;
    }
    lift.set_drive_power(speed);
}

void MoveLiftDegrees::periodic() {

}

bool MoveLiftDegrees::is_complete() {
    if (target_degree > start_degree) {
        return lift.get_degrees() >= target_degree;
    }
    return lift.get_degrees() <= target_degree;
}

void MoveLiftDegrees::shutdown() {
    lift.set_drive_power(0);
    lift.brake();
}





