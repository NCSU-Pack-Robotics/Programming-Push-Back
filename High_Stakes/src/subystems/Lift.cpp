#include "Lift.hpp"

Lift::Lift() : AbstractSubsystem() {

}

void Lift::initialize() {
    lift_motor = std::make_unique<pros::Motor>(Ports::LIFT_MOTOR_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
    lift_motor->tare_position();
    lift_motor->move_velocity(0);
    braking = false;
}

void Lift::periodic() {

    if (braking) {
        brake_now();
        // return because nothing after this matters if it is braking.
        return;
    } else {
        lift_motor->set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    }

    switch (drive_type) {
        case Constants::DriveType::POWER: {
            lift_motor->move(lift_power);
            break;
        }
        case Constants::DriveType::VOLTAGE: {
            lift_motor->move_voltage(lift_voltage);
            break;
        }
    }
}

void Lift::disabled_periodic() {

}

void Lift::shutdown() {
    brake_now();
}

void Lift::set_voltage(int32_t voltage) {
    lift_voltage = std::clamp(voltage, INT32_C(-12000), INT32_C(12000));

    drive_type = Constants::DriveType::VOLTAGE;
}

void Lift::set_drive_power(int32_t power) {
    lift_power = std::clamp(power, INT32_C(-127), INT32_C(127));

    drive_type = Constants::DriveType::POWER;
}

bool Lift::set_braking(const bool braking) {
    // Don't need to do anything if its already in the right mode
    if (braking == this->braking) return braking;
    bool old = this->braking;

    this->braking = braking;
    return old;
}

void Lift::brake_now() {
    lift_motor->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    lift_voltage = 0;
    lift_power = 0;
    lift_motor->brake();
}

int32_t Lift::get_degrees() {
    return lift_motor->get_position();
}
