#include "Intake.hpp"
#include "../ports.hpp"

Intake::Intake() {

}

void Intake::initialize() {
    intake_motor = std::make_unique<pros::Motor>(Ports::INTAKE_MOTOR_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
    intake_motor->tare_position();
    intake_motor->move_velocity(0);
}

void Intake::periodic() {
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
    brake();
}

void Intake::set_voltage(const int32_t voltage) {
    intake_voltage = std::clamp(voltage, INT32_C(-12000), INT32_C(12000));

    drive_type = Constants::DriveType::VOLTAGE;
}

void Intake::set_drive_power(int32_t power) {
    intake_power = std::clamp(power, INT32_C(-127), INT32_C(127));

    drive_type = Constants::DriveType::POWER;
}

void Intake::brake() {
    intake_voltage = 0;
    intake_power = 0;

    intake_motor->move_velocity(0);
}
