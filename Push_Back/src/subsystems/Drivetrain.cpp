#include "Drivetrain.hpp"
#include <numeric>
#include <set>

#include "../ports.hpp"
#include "../math/Utils.hpp"

/** Constant to multiply drive powers by to move robot forward. */
#define FORWARDS 1
/** Constant to multiply drive powers by to move robot backwards. */
#define BACKWARDS (-1)

Drivetrain::Drivetrain() : AbstractSubsystem() {
    this->reversing = false;
    this->braking = false;
    this->direction = FORWARDS;
    this->drive_type = Constants::DriveType::VOLTAGE;

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

    // Construct rotation sensors
    left_rotation_sensor = std::make_unique<pros::Rotation>(Ports::LEFT_ROTATION_SENSOR_PORT);
    right_rotation_sensor = std::make_unique<pros::Rotation>(Ports::RIGHT_ROTATION_SENSOR_PORT);

    // Construct the gyro
    gyro = std::make_unique<pros::Imu>(Ports::GYRO_PORT);

    // Construct initial pose
    Pose initial_pose = {Constants::Initial::Pose::INITIAL_X,
                         Constants::Initial::Pose::INITIAL_Y,
                         Constants::Initial::Pose::INITIAL_HEADING};

    // Initialize odometry
    odometry = std::make_unique<OdometryGyro>(initial_pose);
}

void Drivetrain::initialize() {
    // Set all positions to 0
    left_motors->tare_position_all();
    right_motors->tare_position_all();

    left_rotation_sensor->reset();
    right_rotation_sensor->reset();
    left_rotation_sensor->reset_position();
    right_rotation_sensor->reset_position();

    // Calibrate the gryo
    gyro->reset(true);  // Takes about 2 seconds - max 3 seconds
    gyro->tare();
    gyro->tare_euler();

    reversing = false;
    braking = false;
}

void Drivetrain::periodic() {
    // Calculate the pose of the robot
    const double heading = -this->gyro->get_rotation() * M_PI / 180;
    this->odometry->calculate(this->get_position(), heading);

    if (braking) {
        brake_now();
        // return because nothing after this matters if it is braking.
        return;
    } else {
        left_motors->set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
        right_motors->set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    }

    switch (drive_type) {
        case Constants::DriveType::POWER: {
            if (reversing) {
                right_motors->move(direction * left_drive_power);
                left_motors->move(direction * right_drive_power);
            } else {
                left_motors->move(direction * left_drive_power);
                right_motors->move(direction * right_drive_power);
            }

            break;  // Fuck bitch fuck (NEED THIS)
        }
        case Constants::DriveType::VOLTAGE: {
            if (reversing) {
                right_motors->move_voltage(direction * left_drive_voltage);
                left_motors->move_voltage(direction * right_drive_voltage);
            } else {
                left_motors->move_voltage(direction * left_drive_voltage);
                right_motors->move_voltage(direction * right_drive_voltage);
            }

            break;
        }
    }
}

void Drivetrain::disabled_periodic() {
    // In case robot is moved when disabled
    const double heading = -this->gyro->get_rotation()  * M_PI / 180;
    odometry->calculate(this->get_position(), heading);
}

void Drivetrain::shutdown() {
    brake_now();
}

void Drivetrain::set_voltage(const int32_t left_mV, const int32_t right_mV) {
    left_drive_voltage = std::clamp(left_mV, INT32_C(-12000), INT32_C(12000));
    right_drive_voltage = std::clamp(right_mV, INT32_C(-12000), INT32_C(12000));

    drive_type = Constants::DriveType::VOLTAGE;
}

void Drivetrain::set_drive_power(const int32_t left_power, const int32_t right_power) {
    left_drive_power = std::clamp(left_power, INT32_C(-127), INT32_C(127));
    right_drive_power = std::clamp(right_power, INT32_C(-127), INT32_C(127));

    drive_type = Constants::DriveType::POWER;
}

void Drivetrain::set_velocity(const double target_left_velocity, const double target_right_velocity) {
    // Get current velocities from the motors
    const std::vector<double> left_velocities = left_motors->get_actual_velocity_all();
    const std::vector<double> right_velocities = right_motors->get_actual_velocity_all();

    // Add all left/right motors together and divide by count to get average velocity
    const double left_velocity = Utils::rpm_to_ips(std::reduce(left_velocities.begin(),
    left_velocities.end(), 0.0) / left_velocities.size());
    const double right_velocity = Utils::rpm_to_ips(std::reduce(right_velocities.begin(),
    right_velocities.end(), 0.0) / right_velocities.size());

    // Calculate current error
    const double left_error = target_left_velocity - left_velocity;
    const double right_error = target_right_velocity - right_velocity;

    // Calculate new voltages to set
    const int32_t left_voltage = static_cast<int32_t>(left_velocity_pid.calculate(left_error));
    const int32_t right_voltage = static_cast<int32_t>(right_velocity_pid.calculate(right_error));

    // Set the new voltages
    set_voltage(left_voltage, right_voltage);
}

bool Drivetrain::set_braking(const bool braking) {
    // Don't need to do anything if its already in the right mode
    if (braking == this->braking) return braking;
    bool old = this->braking;

    this->braking = braking;
    return old;
}

void Drivetrain::brake_now() {
    left_motors->set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
    right_motors->set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);

    // Clear old velocities
    left_drive_power = 0;
    right_drive_power = 0;
    right_drive_voltage = 0;
    left_drive_voltage = 0;

    // set braking
    left_motors->brake();
    right_motors->brake();
}

bool Drivetrain::set_reversing(const bool reversing) {
    // Don't need to do anything if its already in the right mode
    if (reversing == this->reversing) return reversing;
    const bool old = this->reversing;

    this->direction = reversing ? BACKWARDS : FORWARDS;

    this->reversing = reversing;
    return old;
}

std::pair<double, double> Drivetrain::get_position(const bool respect_reverse) const {
    // Get the positions of the drivetrain in degrees - https://www.vexforum.com/t/get-angle-vs-get-position-pros/115915
    double left_position  = left_rotation_sensor->get_position() / 100;
    double right_position = right_rotation_sensor->get_position() / 100;

    // Reverse values if we are in reverse mode
    if (respect_reverse && reversing) {
        left_position = -left_position;
        right_position = -right_position;

        return {right_position, left_position};
    }

    return {left_position, right_position};
}

Pose Drivetrain::get_pose() const {
    Pose pose = this->odometry->get_pose();

    if (reversing) pose.heading += M_PI;

    return pose;
}

void Drivetrain::set_pose(const Pose new_pose) const {
    this->odometry->set_pose(new_pose);
}
