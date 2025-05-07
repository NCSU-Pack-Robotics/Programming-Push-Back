#include "DriveStraightIntakeLift.hpp"

DriveStraightIntakeLift::DriveStraightIntakeLift(const double inches, const double tolerance, const double max_time) :
    DriveStraight(inches, tolerance, max_time) {

}

void DriveStraightIntakeLift::initialize() {
    DriveStraight::initialize();

    // Turn on lift and intake
    lift.set_drive_power(Constants::Controller::MotorSpeeds::LIFT_UP);
    intake.set_drive_power(Constants::Controller::MotorSpeeds::INTAKE_INWARDS);
    intake.set_braking(false);
    lift.set_braking(false);
}

void DriveStraightIntakeLift::shutdown() {
    DriveStraight::shutdown();

    // Stop intake and lift
    lift.set_braking(true);
    intake.set_braking(true);
    intake.set_drive_power(0);
    lift.set_drive_power(0);
}
