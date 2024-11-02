#include "Lift.hpp"

Lift::Lift() {

}

void Lift::initialize() {
    // TODO: Make sure motor is correct color

    lift_motor = std::make_unique<pros::Motor>(Ports::LIFT_MOTOR_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
    lift_motor->tare_position(0);
    lift_motor->set
}

void Lift::periodic() {
    switch (drive_type) {
        case DriveType::Power: {
            lift_motor->move(lift_power);
            break;
        },
        case DriveType::Voltage: {
            lift_motor->move_voltage(lift_voltage);
            break;
        }
    }
}

void Lift::disabled_periodic() {
    // TODO: Stop the motors

}

void Lift::shutdown() {
    // TODO: Stop the motors
}

void Lift::set_voltage(int32_t voltage) {
    lift_voltage = std::clamp(voltage, INT32_C(-12000), INT32_C(12000));

    drive_type = DriveType::VOLTAGE;
}

void Lift::set_drive_power(int32_t power) {
    lift_power = std::clamp(power, INT32_C(-127), INT32_C(127));

    drive_type = DriveType::POWER;
}


