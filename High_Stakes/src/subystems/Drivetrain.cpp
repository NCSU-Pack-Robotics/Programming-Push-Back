#include "Drivetrain.hpp"
#include <numeric>
#include "../ports.hpp"

Drivetrain::Drivetrain() : AbstractSubsystem() {
}

void Drivetrain::initialize() {
    // Initialize motor objects:
    left_front1 = std::make_unique<pros::Motor>(Ports::LEFT_FRONT1_MOTOR_PORT,
                                               pros::v5::MotorGears::blue,
                                               pros::v5::MotorUnits::degrees);
    left_front2 = std::make_unique<pros::Motor>(Ports::LEFT_FRONT2_MOTOR_PORT,
                                               pros::v5::MotorGears::blue,
                                               pros::v5::MotorUnits::degrees);
    left_back2 = std::make_unique<pros::Motor>(Ports::LEFT_BACK2_MOTOR_PORT,
                                                pros::v5::MotorGears::blue,
                                                pros::v5::MotorUnits::degrees);
    left_back1 = std::make_unique<pros::Motor>(Ports::LEFT_BACK1_MOTOR_PORT,
                                                pros::v5::MotorGears::blue,
                                                pros::v5::MotorUnits::degrees);

    right_front1 = std::make_unique<pros::Motor>(Ports::RIGHT_FRONT1_MOTOR_PORT,
                                               pros::v5::MotorGears::blue,
                                               pros::v5::MotorUnits::degrees);
    right_front2 = std::make_unique<pros::Motor>(Ports::RIGHT_FRONT2_MOTOR_PORT,
                                               pros::v5::MotorGears::blue,
                                               pros::v5::MotorUnits::degrees);
    right_back2 = std::make_unique<pros::Motor>(Ports::RIGHT_BACK2_MOTOR_PORT,
                                                pros::v5::MotorGears::blue,
                                                pros::v5::MotorUnits::degrees);
    right_back1 = std::make_unique<pros::Motor>(Ports::RIGHT_BACK1_MOTOR_PORT,
                                                pros::v5::MotorGears::blue,
                                                pros::v5::MotorUnits::degrees);

    // Initialize motor groups:
    left_motors = std::make_unique<pros::MotorGroup>
            (std::initializer_list<std::int8_t>{left_front1->get_port(), left_front2->get_port(), left_back2->get_port(), left_back1->get_port()},
             left_front1->get_gearing(),
             left_front1->get_encoder_units());
    right_motors = std::make_unique<pros::MotorGroup>
            (std::initializer_list<std::int8_t>{right_front1->get_port(), right_front2->get_port(), right_back2->get_port(), right_back1->get_port()},
             right_front1->get_gearing(),
             right_front1->get_encoder_units());

    // Set all positions to 0
    left_motors->tare_position_all();
    right_motors->tare_position_all();

    // Ensure motors are stopped
    left_motors->move_velocity(0);
    right_motors->move_velocity(0);

    // Set the brake mode on the motors
    left_motors->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    right_motors->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

    // Construct initial pose
    Pose initial_pose = {Constants::Initial::Pose::INITIAL_X,
                         Constants::Initial::Pose::INITIAL_Y,
                         Constants::Initial::Pose::INITIAL_HEADING};

    // Initialize calculate
     odometry = std::make_unique<Odometry>(initial_pose, *this);
}

void Drivetrain::periodic() {
    // Calculate the pose of the robot
    this->odometry->calculate();
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

void Drivetrain::brake() {
    left_motors->brake();
    right_motors->brake();
    set_drive_power(0,0);
}

void Drivetrain::set_velocity(const double target_left_velocity, const double target_right_velocity) {
    // Get current velocities from the motors
    const std::vector<double> left_velocities = left_motors->get_actual_velocity_all();
    const std::vector<double> right_velocities = right_motors->get_actual_velocity_all();

    // Add all left/right motors together and divide by count to get average velocity
    const double left_velocity = rpm_to_ips(std::reduce(left_velocities.begin(), left_velocities.end(), 0.0) / left_velocities.size());
    const double right_velocity = rpm_to_ips(std::reduce(right_velocities.begin(), right_velocities.end(), 0.0) / right_velocities.size());

    // Calculate current error
    const double left_error = target_left_velocity - left_velocity;
    const double right_error = target_right_velocity - right_velocity;

    // Calculate new voltages to set
    const auto left_voltage = static_cast<int32_t>(left_velocity_pid.calculate(left_error));
    const auto right_voltage = static_cast<int32_t>(right_velocity_pid.calculate(right_error));

    // Set the new voltages
    set_voltage(left_voltage, right_voltage);
}

std::pair<double, double> Drivetrain::get_position() const {
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

Pose Drivetrain::get_pose() const {
    return this->odometry->get_pose();
}
