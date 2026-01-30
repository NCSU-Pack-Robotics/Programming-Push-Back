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
     * Set the voltage (in milli-volts) of the motors.
     * Input values are clamped to the range [-12000,12000]
     * @param front_left The voltage to set the front left motors to.
     * @param front_right The voltage to set the front right motors to.
     * @param back_right The voltage to set the back right motors to.
     * @param back_left The voltage to set the back left motors to.
     */
    void set_voltage(int32_t front_left, int32_t front_right, int32_t back_right, int32_t back_left);

    /**
     * Set the power of the motors.
     * Input values are clamped to the range [-127, 127]
     * @param front_left The power to set the front left motors to.
     * @param front_right The power to set the front right motors to.
     * @param back_right The power to set the back right motors to.
     * @param back_left The power to set the back left motors to.
     */
    void set_drive_power(int32_t front_left, int32_t front_right, int32_t back_right, int32_t back_left);

    /**
     * Set the motors to run at a target velocity in inches per second
     * @param front_left The velocity to set the front left motors to.
     * @param front_right The velocity to set the front left motors to.
     * @param back_right The velocity to set the back right motors to.
     * @param back_left The velocity to set the back left motors to.
     */
    void set_velocity(double front_left, double front_right, double back_right, double back_left);

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
    // std::pair<double, double> get_position(bool respect_reverse=false) const;

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
    void set_pose(const Pose& new_pose);

private:
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t front_left_voltage = 0;
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t front_right_voltage = 0;
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t back_right_voltage = 0;
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t back_left_voltage = 0;


    /** The current braking state of the robot */
    bool braking;
    /** The current reversal state of the robot */
    bool reversing;
    /** The direction the robot is moving in. 1 for forward, -1 for reverse */
    int direction;

    OdometryGyro odometry;

    /** Power to set motors to from analog sticks. Will be between [-127, 127] */
    int32_t front_left_power = 0;
    /** Power to set motors to from analog sticks. Will be between [-127, 127] */
    int32_t front_right_power = 0;
    /** Power to set motors to from analog sticks. Will be between [-127, 127] */
    int32_t back_right_power = 0;
    /** Power to set motors to from analog sticks. Will be between [-127, 127] */
    int32_t back_left_power = 0;

    /** The PID used for front left motors velocity */
    PID front_left_velocity_pid{Constants::PID::Drive::Velocity::Kp,
                                Constants::PID::Drive::Velocity::Kd,
                                Constants::PID::Drive::Velocity::Ki};
    /** The PID used for front right motors velocity */
    PID front_right_velocity_pid{Constants::PID::Drive::Velocity::Kp,
                                 Constants::PID::Drive::Velocity::Kd,
                                 Constants::PID::Drive::Velocity::Ki};
    /** The PID used for back right motors velocity */
    PID back_right_velocity_pid{Constants::PID::Drive::Velocity::Kp,
                                Constants::PID::Drive::Velocity::Kd,
                                Constants::PID::Drive::Velocity::Ki};
    /** The PID used for back left motors velocity */
    PID back_left_velocity_pid{Constants::PID::Drive::Velocity::Kp,
                                 Constants::PID::Drive::Velocity::Kd,
                                 Constants::PID::Drive::Velocity::Ki};


    /** The PID used for turning */
    PID angular_pid{Constants::PID::Drive::Angular::Kp,
                          Constants::PID::Drive::Angular::Kd,
                          Constants::PID::Drive::Angular::Ki};

    /** Type of drive control to use. */
    Constants::DriveType drive_type;



    // Motors:
    /** The left, front motor closer to the ground. */
    pros::Motor left_front1;
    /** The left, front motor farther from the ground. */
    pros::Motor left_front2;

    /** The right, front motor closer to the ground. */
    pros::Motor right_front1;
    /** The right, front motor farther from the ground. */
    pros::Motor right_front2;

    /** The right, back motor closer to the ground. */
    pros::Motor right_back1;
    /** The right, back motor farther from the ground. */
    pros::Motor right_back2;

    /** The left, back motor closer to the ground. */
    pros::Motor left_back1;
    /** The left, back motor farther from the ground. */
    pros::Motor left_back2;




    // Motor groups:
    /** Group of all the motors on the front left of the robot. */
    pros::MotorGroup front_left_motors;
    /** Group of all the motors on the front right of the robot. */
    pros::MotorGroup front_right_motors;
    /** Group of all the motors on the bakc right of the robot. */
    pros::MotorGroup back_right_motors;
    /** Group of all the motors on the back left of the robot. */
    pros::MotorGroup back_left_motors;

    // Rotation sensors:
    /** Smart pointer to the left rotation sensor. */
    // pros::Rotation left_rotation_sensor;
    /** Smart pointer to the right rotation sensor. */
    // pros::Rotation right_rotation_sensor;

    // Gyro:
    /** Smart pointer to the gyro sensor. */
    // pros::Imu gyro;

protected:
    Drivetrain();
};
