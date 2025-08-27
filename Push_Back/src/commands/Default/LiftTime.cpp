#include "LiftTime.hpp"

#include "../../Config.hpp"

LiftTime::LiftTime(double time, double speed, double direction) : Command(), time(time), speed(speed), direction(direction) {
};

void LiftTime::initialize() {
    // Start the timer
    timer.start();

    // Start the lift
    lift.set_drive_power(direction * this->speed);
}

void LiftTime::periodic() {
}

void LiftTime::shutdown() {
    // Stop the lift
    lift.set_drive_power(0);
}

bool LiftTime::is_complete() {
    return timer.get_duration() >= time;
}