#include "Drivetrain.hpp"
#include <numeric>
#include <set>

#include "../ports.hpp"
#include "../math/Utils.hpp"

/** Enum to multiply drive powers by to move robot forward. */
enum Direction {
    BACKWARDS = -1,
    FORWARDS = 1
};

Drivetrain::Drivetrain() :
    AbstractSubsystem(),
    odometry({Constants::Initial::Pose::INITIAL_X, Constants::Initial::Pose::INITIAL_Y, Constants::Initial::Pose::INITIAL_HEADING}),

    left_front1(Ports::LEFT_FRONT1_MOTOR_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees),
    left_front2(Ports::LEFT_FRONT2_MOTOR_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees),

    right_front1(Ports::RIGHT_FRONT1_MOTOR_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees),
    right_front2(Ports::RIGHT_FRONT2_MOTOR_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees),

    right_back1(Ports::RIGHT_BACK1_MOTOR_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees),
    right_back2(Ports::RIGHT_BACK2_MOTOR_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees),

    left_back1(Ports::LEFT_BACK1_MOTOR_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees),
    left_back2(Ports::LEFT_BACK2_MOTOR_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees),

    front_left_motors({left_front1.get_port(), left_front2.get_port()},
               left_front1.get_gearing(),
               left_front1.get_encoder_units()),

    front_right_motors({right_front1.get_port(), right_front2.get_port()},
                right_front1.get_gearing(),
                right_front1.get_encoder_units()),

    back_right_motors({right_back1.get_port(), right_back2.get_port()},
               right_back1.get_gearing(),
               right_back1.get_encoder_units()),

    back_left_motors({left_back1.get_port(), left_back2.get_port()},
              left_back1.get_gearing(),
              left_back1.get_encoder_units())

    // left_rotation_sensor(Ports::LEFT_ROTATION_SENSOR_PORT),
    // right_rotation_sensor(Ports::RIGHT_ROTATION_SENSOR_PORT),

    // gyro(Ports::GYRO_PORT)
{
    this->reversing = false;
    this->braking = false;
    this->direction = Direction::FORWARDS;
    this->drive_type = Constants::DriveType::VOLTAGE;
}

void Drivetrain::initialize() {
    // Set all positions to 0
    front_left_motors.tare_position_all();
    front_right_motors.tare_position_all();
    back_right_motors.tare_position_all();
    back_right_motors.tare_position_all();

    // left_rotation_sensor.reset();
    // right_rotation_sensor.reset();
    // left_rotation_sensor.reset_position();
    // right_rotation_sensor.reset_position();

    // Calibrate the gryo
    // gyro.reset(true);  // Takes about 2 seconds - max 3 seconds
    // gyro.tare();
    // gyro.tare_euler();

    reversing = false;
    braking = false;
}

void Drivetrain::periodic() {
    // Calculate the pose of the robot
    // const double heading = -this->gyro.get_rotation() * M_PI / 180;
    // this->odometry.calculate(this->get_position(), heading);

    if (braking) {
        brake_now();
        // return because nothing after this matters if it is braking.
        return;
    } else {
        front_left_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
        front_right_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
        back_right_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
        back_left_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    }

    switch (drive_type) {
        case Constants::DriveType::POWER: {
            // if (reversing) {
                // right_motors.move(direction * left_drive_power);
                // left_motors.move(direction * right_drive_power);
            // } else {
                // printf("Setting power")
                front_left_motors.move(direction * front_left_power);
                front_right_motors.move(direction * front_right_power);
                back_right_motors.move(direction * back_right_power);
                back_left_motors.move(direction * back_left_power);
            // }

            break;  // Fuck bitch fuck (NEED THIS)
        }
        case Constants::DriveType::VOLTAGE: {
            // if (reversing) {
                // right_motors.move_voltage(direction * left_drive_voltage);
                // left_motors.move_voltage(direction * right_drive_voltage);
            // } else {
                front_left_motors.move_voltage(direction * front_left_voltage);
                front_right_motors.move_voltage(direction * front_right_voltage);
                back_right_motors.move_voltage(direction * back_right_voltage);
                back_left_motors.move_voltage(direction * back_left_voltage);
            // }

            break;
        }
    }
}

void Drivetrain::disabled_periodic() {
    // In case robot is moved when disabled
    // const double heading = -this->gyro.get_rotation()  * M_PI / 180;
    // this->odometry.calculate(this->get_position(), heading);
}

void Drivetrain::shutdown() {
    brake_now();
}

void Drivetrain::set_voltage(int32_t front_left, int32_t front_right, int32_t back_right, int32_t back_left) {
    front_left_voltage = std::clamp(front_left, INT32_C(-12000), INT32_C(12000));
    front_right_voltage = std::clamp(front_right, INT32_C(-12000), INT32_C(12000));
    back_right_voltage = std::clamp(back_right, INT32_C(-12000), INT32_C(12000));
    back_left_voltage = std::clamp(back_left, INT32_C(-12000), INT32_C(12000));

    drive_type = Constants::DriveType::VOLTAGE;
}

void Drivetrain::set_drive_power(int32_t front_left, int32_t front_right, int32_t back_right, int32_t back_left) {
    front_left_power = std::clamp(front_left, INT32_C(-127), INT32_C(127));
    front_right_power = std::clamp(front_right, INT32_C(-127), INT32_C(127));
    back_right_power = std::clamp(back_right, INT32_C(-127), INT32_C(127));
    back_left_power = std::clamp(back_left, INT32_C(-127), INT32_C(127));

    drive_type = Constants::DriveType::POWER;
}

void Drivetrain::set_velocity(double front_left, double front_right, double back_right, double back_left) {
    // Get current velocities from the motors
    const std::vector<double> front_left_vel = front_left_motors.get_actual_velocity_all();
    const std::vector<double> front_right_vel = front_right_motors.get_actual_velocity_all();
    const std::vector<double> back_right_vel = back_right_motors.get_actual_velocity_all();
    const std::vector<double> back_left_vel = back_left_motors.get_actual_velocity_all();

    // Add all left/right motors together and divide by count to get average velocity
    const double left_front_avg = Utils::rpm_to_ips(std::reduce(front_left_vel.begin(),
    front_left_vel.end(), 0.0) / front_left_vel.size());
    const double right_front_avg = Utils::rpm_to_ips(std::reduce(front_right_vel.begin(),
    front_right_vel.end(), 0.0) / front_right_vel.size());
    const double back_right_avg = Utils::rpm_to_ips(std::reduce(back_right_vel.begin(),
    back_right_vel.end(), 0.0) / back_right_vel.size());
    const double back_left_avg = Utils::rpm_to_ips(std::reduce(back_left_vel.begin(),
    back_left_vel.end(), 0.0) / back_left_vel.size());

    // Calculate current error
    const double front_left_error = front_left - left_front_avg;
    const double front_right_error = front_right - right_front_avg;
    const double back_right_error = back_right - back_right_avg;
    const double back_left_error = back_left - back_left_avg;

    // Calculate new voltages to set
    const int32_t front_left_voltage = static_cast<int32_t>(front_left_velocity_pid.calculate(front_left_error));
    const int32_t front_right_voltage = static_cast<int32_t>(front_right_velocity_pid.calculate(front_right_error));
    const int32_t back_right_voltage = static_cast<int32_t>(back_right_velocity_pid.calculate(back_right_error));
    const int32_t back_left_voltage = static_cast<int32_t>(back_left_velocity_pid.calculate(back_left_error));

    // Set the new voltages
    set_voltage(front_left_voltage, front_right_voltage, back_right_voltage, back_left_voltage);
}

bool Drivetrain::set_braking(const bool braking) {
    // Don't need to do anything if its already in the right mode
    if (braking == this->braking) return braking;
    bool old = this->braking;

    this->braking = braking;
    return old;
}

void Drivetrain::brake_now() {
    front_left_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
    front_right_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
    back_right_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
    back_left_motors.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);

    // Clear old velocities
    front_left_power = 0;
    front_right_power = 0;
    back_right_power = 0;
    back_left_power = 0;

    front_left_voltage = 0;
    front_right_voltage = 0;
    back_right_voltage = 0;
    back_left_voltage = 0;

    // set braking
    front_left_motors.brake();
    front_right_motors.brake();
    back_right_motors.brake();
    back_left_motors.brake();
}

bool Drivetrain::set_reversing(const bool reversing) {
    // Don't need to do anything if its already in the right mode
    if (reversing == this->reversing) return reversing;
    const bool old = this->reversing;

    this->direction = reversing ? Direction::BACKWARDS : Direction::FORWARDS;

    this->reversing = reversing;
    return old;
}

// std::pair<double, double> Drivetrain::get_position(const bool respect_reverse) const {
//     // Get the positions of the drivetrain in degrees - https://www.vexforum.com/t/get-angle-vs-get-position-pros/115915
//     double left_position  = left_rotation_sensor.get_position() / 100.0;
//     double right_position = right_rotation_sensor.get_position() / 100.0;
//
//     // Reverse values if we are in reverse mode
//     if (respect_reverse && reversing) {
//         left_position = -left_position;
//         right_position = -right_position;
//
//         return {right_position, left_position};
//     }
//
//     return {left_position, right_position};
// }

Pose Drivetrain::get_pose() const {
    Pose pose = this->odometry.get_pose();

    if (reversing) pose.heading += M_PI;

    return pose;
}

void Drivetrain::set_pose(const Pose& new_pose) {
    this->odometry.set_pose(new_pose);
}
