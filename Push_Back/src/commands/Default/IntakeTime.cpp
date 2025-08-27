#include "IntakeTime.hpp"

#include "../../Config.hpp"

IntakeTime::IntakeTime(double time, double speed, double direction) : Command(), time(time), speed(speed), direction(direction) {
};

void IntakeTime::initialize() {
    // Start the timer
    timer.start();

    // Start the intake
    intake.set_drive_power(direction * this->speed);
}

void IntakeTime::periodic() {
}

void IntakeTime::shutdown() {
    // Stop the intake
    intake.set_drive_power(0);
}

bool IntakeTime::is_complete() {
    return timer.get_duration() >= time;
}

