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
    // Get the position of the left and right motors
    std::pair<double, double> position = get_position();

    // Calculate the pose of the robot
    this->pose = odometry(position.first, position.second);
}

void Drivetrain::disabled_periodic() {

}

void Drivetrain::shutdown() {

}

void Drivetrain::set_voltage(int32_t left_mV, int32_t right_mV) {

}

void Drivetrain::set_drive_power(int32_t leftPower, int32_t rightPower) {

}

std::pair<double, double> Drivetrain::get_position() {
    // Average both motor positions to be more accurate
    double left_position = (left_front->get_position() + left_back->get_position()) / 2;
    double right_position = (right_front->get_position() + right_back->get_position()) / 2;

    // Return the pair of positions
    return std::make_pair(left_position, right_position);
}

Pose Drivetrain::get_pose() {
    return this->pose;
}

Pose Drivetrain::odometry(double left_position, double right_position) {
    return Pose();
}
