#pragma once

#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../Config.hpp"
#include "../Constants.hpp"
#include "../Pose.hpp"
#include "../math/PID.hpp"
#include "../math/odometry/OdometryGyro.hpp"


/**
 * The Drivetrain subsystem is responsible for controlling and reading from the drive motors.
 */
class Drivetrain : public AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

    /**
     * Set the voltage (in milli-volts) of the left and right motors in range [-12000,12000].
     * @param left_mV The voltage to set the left motors to.
     * @param right_mV The voltage to set the right motors to.
     */
    void set_voltage(int32_t left_mV, int32_t right_mV);

    /**
     * Set the power of the left and right motors in range [-127, 127].
     * @param left_power The power to set the left motors to.
     * @param right_power The power to set the right motors to.
     */
    void set_drive_power(int32_t left_power, int32_t right_power);

    /**
     * Set the left motors to run at a target velocity in inches per second
     * @param target_left_velocity The velocity of the left motors in inches per second
     * @param target_right_velocity The velocity of the right motors in inches per second
     */
    void set_velocity(double target_left_velocity, double target_right_velocity);

    /** Sets the robot to be braking. When it is in this state all methods that would usually move the robot will not work. When this method is called the robot is not
     * guaranteed to be braked, it will become braked when the next periodic loop runs if braking is still true.
     * @returns The previous brake state.
     */
    bool set_braking(bool braking);

    /**
     * Immediately stops the robot, setting all power and voltage power to 0.
     * This is an instantaneous setting of the brake, and should not be used unless you know it won't be reset
     * in the next periodic loop.
     */
    void brake_now();

    /**
     * Sets the robot to be reversing. All motor inputs will be reversed.
     * This is done by setting an internal flag and by setting motors to -1 * the power/voltage.
     * @returns The previous reversal state.
     */
    bool set_reversing(bool reversing);

    /**
     * Get the position of the left and right motors in degrees.
     * @param respect_reverse If true, the returned values will be negated and swapped. If false,
     * the returned value will be absolute to the original orientation of the robot.
     * Ex: This should be true if dealing with position deltas, and you want to pass positive or
     * negative values to some sort of PID.
     * @return A pair of the left and right motor positions in degrees. The first value is the left
     * motor position, and the second value is the right motor position.
     */
    std::pair<double, double> get_position(bool respect_reverse=false) const;

    /**
    * Get the pose of the robot.
    * This is the result of odometer calculations.
    * @return The pose of the robot.
    */
    Pose get_pose() const;

    /**
     * Set the pose of the robot.
     * This method is intended to be used when the robot hits a wall to realign itself.
     * @param new_pose The new pose of the robot.
     */
    void set_pose(Pose new_pose) const;

private:
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t left_drive_voltage = 0;
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t right_drive_voltage = 0;

    /** The current braking state of the robot */
    bool braking;
    /** The current reversal state of the robot */
    bool reversing;
    /** The direction the robot is moving in. 1 for forward, -1 for reverse */
    int direction;

    // Pointer to calculate instance
    std::unique_ptr<OdometryGyro> odometry;

    /** Power to set motors to from analog sticks. Will be between -127 and 127 */
    int32_t left_drive_power = 0;
    /** Power to set motors to from analog sticks. Will be between -127 and 127 */
    int32_t right_drive_power = 0;

    /** The PID used for left motors velocity */
    PID left_velocity_pid = PID(Constants::PID::Drive::Velocity::Kp,
                                Constants::PID::Drive::Velocity::Kd,
                                Constants::PID::Drive::Velocity::Ki);
    /** The PID used for right motors velocity */
    PID right_velocity_pid = PID(Constants::PID::Drive::Velocity::Kp,
                                 Constants::PID::Drive::Velocity::Kd,
                                 Constants::PID::Drive::Velocity::Ki);
    /** The PID used for turning */
    PID angular_pid = PID(Constants::PID::Drive::Angular::Kp,
                          Constants::PID::Drive::Angular::Kd,
                          Constants::PID::Drive::Angular::Ki);

    /** Type of drive control to use. */
    Constants::DriveType drive_type;

    // Motors:
    /** Smart pointer to the left, front motor. */
    std::unique_ptr<pros::Motor> left_front1;
    /** Smart pointer to the left, front motor. */
    std::unique_ptr<pros::Motor> left_front2;
    /** Smart pointer to the left, front motor. */
    std::unique_ptr<pros::Motor> left_back2;
    /** Smart pointer to the left, front motor. */
    std::unique_ptr<pros::Motor> left_back1;

    /** Smart pointer to the left, front motor. */
    std::unique_ptr<pros::Motor> right_front1;
    /** Smart pointer to the left, front motor. */
    std::unique_ptr<pros::Motor> right_front2;
    /** Smart pointer to the left, front motor. */
    std::unique_ptr<pros::Motor> right_back2;
    /** Smart pointer to the left, front motor. */
    std::unique_ptr<pros::Motor> right_back1;

    // Motor groups:
    /** Group of all motors on the left side of the robot. */
    std::unique_ptr<pros::MotorGroup> left_motors;
    /** Group of all motors on the right side of the robot. */
    std::unique_ptr<pros::MotorGroup> right_motors;

    // Rotation sensors:
    /** Smart pointer to the left rotation sensor. */
    std::unique_ptr<pros::Rotation> left_rotation_sensor;
    /** Smart pointer to the right rotation sensor. */
    std::unique_ptr<pros::Rotation> right_rotation_sensor;

    // Gyro:
    /** Smart pointer to the gyro sensor. */
    std::unique_ptr<pros::Imu> gyro;

protected:
    /**
     * Constructor for Drivetrain subsystem.
     * Takes no arguments.
     * Can be called as: <code>Drivetrain drivetrain = Drivetrain();</code>
     */
    Drivetrain();
};

