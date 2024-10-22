#include "Drivetrain.hpp"

Drivetrain::Drivetrain() : AbstractSubsystem() {
}

void Drivetrain::initialize() {
    // TODO: Ensure motors are using green gear-set


    // Initialize motor objects:
    left_front = std::make_unique<pros::Motor>(Ports::LEFT_FRONT_MOTOR_PORT,
                                               pros::v5::MotorGears::green,
                                               pros::v5::MotorUnits::degrees);
    right_front = std::make_unique<pros::Motor>(Ports::RIGHT_FRONT_MOTOR_PORT,
                                                pros::v5::MotorGears::green,
                                                pros::v5::MotorUnits::degrees);
    left_back = std::make_unique<pros::Motor>(Ports::LEFT_BACK_MOTOR_PORT,
                                                pros::v5::MotorGears::green,
                                                pros::v5::MotorUnits::degrees);
    right_back = std::make_unique<pros::Motor>(Ports::RIGHT_BACK_MOTOR_PORT,
                                                pros::v5::MotorGears::green,
                                                pros::v5::MotorUnits::degrees);

    // Initialize motor groups:
    left_motors = std::make_unique<pros::MotorGroup>
            (std::initializer_list<std::int8_t>{left_front->get_port(), left_back->get_port()},
             left_front->get_gearing(),
             left_front->get_encoder_units());
    right_motors = std::make_unique<pros::MotorGroup>
            (std::initializer_list<std::int8_t>{right_front->get_port(), right_back->get_port()},
             right_front->get_gearing(),
             right_front->get_encoder_units());

    // Set all positions to 0
    left_front->tare_position();
    right_front->tare_position();
    left_back->tare_position();
    right_back->tare_position();

    // Ensure motors are stopped
    left_motors->move_velocity(0);
    right_motors->move_velocity(0);
}

void Drivetrain::periodic() {

}

void Drivetrain::disabled_periodic() {

}

void Drivetrain::shutdown() {

}

void Drivetrain::set_voltage(int32_t left_mV, int32_t right_mV) {
    left_drive_voltage = left_mV;
    right_drive_voltage = right_mV;
}

void Drivetrain::set_drive_power(double left_power, double right_power) {
    left_drive_power = left_power;
    right_drive_power = right_power;
}

std::pair<double, double> Drivetrain::get_position() {
    // Average both motor positions to be more accurate
    double left_position = (left_front->get_position() + left_back->get_position()) / 2;
    double right_position = (right_front->get_position() + right_back->get_position()) / 2;

    // Return the pair of positions
    return std::make_pair(left_position, right_position);
}

