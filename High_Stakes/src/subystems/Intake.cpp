#include "Intake.hpp"
#include "../ports.hpp"

Intake::Intake() {

}

void Intake::initialize() {
    intake_motor = std::make_unique<pros::Motor>(Ports::INTAKE_MOTOR_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
    intake_motor->tare_position();
    intake_motor->move_velocity(0);
    braking = false;
}

void Intake::periodic() {

    if (braking) {
        brake_now();
        // return because nothing after this matters if it is braking.
        return;
    } else {
        intake_motor->set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    }

    switch (drive_type) {
        case Constants::DriveType::POWER: {
            intake_motor->move(intake_power);
            break;
        }
        case Constants::DriveType::VOLTAGE: {
            intake_motor->move_voltage(intake_voltage);
            break;
        }
    }
}

void Intake::disabled_periodic() {

}

void Intake::shutdown() {
    brake_now();
}

void Intake::set_voltage(const int32_t voltage) {
    intake_voltage = std::clamp(voltage, INT32_C(-12000), INT32_C(12000));

    drive_type = Constants::DriveType::VOLTAGE;
}

void Intake::set_drive_power(int32_t power) {
    intake_power = std::clamp(power, INT32_C(-127), INT32_C(127));

    drive_type = Constants::DriveType::POWER;
}

bool Intake::set_braking(const bool braking) {
    // Don't need to do anything if its already in the right mode
    if (braking == this->braking) return braking;
    bool old = this->braking;

    this->braking = braking;
    return old;
}

void Intake::brake_now() {
    intake_motor->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    intake_voltage = 0;
    intake_power = 0;
    intake_motor->brake();
}
