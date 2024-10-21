#include "Drivetrain.hpp"

Drivetrain::Drivetrain() : SubsystemAbstract() {

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
}

void Drivetrain::initialize() {

}

void Drivetrain::periodic() {

}

void Drivetrain::disabled_periodic() {

}

void Drivetrain::shutdown() {

}
