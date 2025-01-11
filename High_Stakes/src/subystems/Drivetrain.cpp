#include "Drivetrain.hpp"
#include <numeric>
#include "../math/Odometry.hpp"
#include "../ports.hpp"
#include "../Config.hpp"

Drivetrain::Drivetrain() : AbstractSubsystem() {
}

void Drivetrain::initialize() {
    // Initialize motor objects:
    left_front = std::make_unique<pros::Motor>(Ports::LEFT_FRONT_MOTOR_PORT,
                                               pros::v5::MotorGears::blue,
                                               pros::v5::MotorUnits::degrees);
    right_front = std::make_unique<pros::Motor>(Ports::RIGHT_FRONT_MOTOR_PORT,
                                                pros::v5::MotorGears::blue,
                                                pros::v5::MotorUnits::degrees);
    left_back = std::make_unique<pros::Motor>(Ports::LEFT_BACK_MOTOR_PORT,
                                                pros::v5::MotorGears::blue,
                                                pros::v5::MotorUnits::degrees);
    right_back = std::make_unique<pros::Motor>(Ports::RIGHT_BACK_MOTOR_PORT,
                                                pros::v5::MotorGears::blue,
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

    // Construct initial pose
    Pose initial_pose = {Constants::Initial::Pose::INITIAL_X,
                         Constants::Initial::Pose::INITIAL_Y,
                         Constants::Initial::Pose::INITIAL_HEADING};

    // Initialize calculate
    // odometry = std::make_unique<Odometry>(initial_pose, *this);
}

void Drivetrain::periodic() {
    // Get the position of the left and right motors
    std::pair<double, double> position = get_position();

    // Calculate the pose of the robot
    // this->odometry->calculate(position.first, position.second);
    switch (drive_type) {
        case Constants::DriveType::POWER: {
            left_motors->move(left_drive_power);
            right_motors->move(right_drive_power);

            break;  // Fuck bitch fuck (NEED THIS)
        }
        case Constants::DriveType::VOLTAGE: {
            left_motors->move_voltage(left_drive_voltage);
            right_motors->move_voltage(right_drive_voltage);

            break;
        }
    }
}

void Drivetrain::disabled_periodic() {
    // TODO: Stop the motors

}

void Drivetrain::shutdown() {
    // TODO: Stop the motors
}

void Drivetrain::set_voltage(int32_t left_mV, int32_t right_mV) {
    left_drive_voltage = std::clamp(left_mV, INT32_C(-12000), INT32_C(12000));
    right_drive_voltage = std::clamp(right_mV, INT32_C(-12000), INT32_C(12000));
    
    drive_type = Constants::DriveType::VOLTAGE;
}

void Drivetrain::set_drive_power(int32_t left_power, int32_t right_power) {
    left_drive_power = std::clamp(left_power, INT32_C(-127), INT32_C(127));
    right_drive_power = std::clamp(right_power, INT32_C(-127), INT32_C(127));

    drive_type = Constants::DriveType::POWER;
}

void Drivetrain::set_velocity(const double target_left_velocity, const double target_right_velocity) {
    // Get current velocities from the motors
    const std::vector<double> left_velocities = left_motors->get_actual_velocity_all();
    const std::vector<double> right_velocities = right_motors->get_actual_velocity_all();

    // Add all left/right motors together and divide by count to get average velocity
    const double left_velocity = rpm_to_ips(std::reduce(left_velocities.begin(), left_velocities.end(), 0.0) / left_velocities.size());
    const double right_velocity = rpm_to_ips(std::reduce(right_velocities.begin(), right_velocities.end(), 0.0) / right_velocities.size());

    // Calculate current error
    double left_error = target_left_velocity - left_velocity;
    double right_error = target_right_velocity - right_velocity;

    // Calculate new voltages to set
    const auto left_voltage = static_cast<int32_t>(left_velocity_pid.calculate(left_error));
    const auto right_voltage = static_cast<int32_t>(right_velocity_pid.calculate(right_error));

    // Set the new voltages
    set_voltage(left_voltage, right_voltage);
}

std::pair<double, double> Drivetrain::get_position() {
    // Get current positions from the motors
    const std::vector<double> left_positions = left_motors->get_position_all();
    const std::vector<double> right_positions = right_motors->get_position_all();

    // Average both motor positions to be more accurate
    double left_position = std::reduce(left_positions.begin(), left_positions.end(), 0.0) / left_positions.size();
    double right_position = std::reduce(right_positions.begin(), right_positions.end(), 0.0) / right_positions.size();

    return std::make_pair(left_position, right_position);
}

double Drivetrain::rpm_to_ips(double const rpm) {
    return rpm * Constants::Hardware::TRACKING_DIAMETER * Constants::Math::PI * Constants::Hardware::TRACKING_RATIO / 60;
}

// Pose Drivetrain::get_pose() {
   //  return this->odometry->get_pose();
// }
