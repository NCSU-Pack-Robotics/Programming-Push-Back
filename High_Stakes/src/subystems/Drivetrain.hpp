#pragma once

#include <cstdint>

#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../Pose.hpp"
#include "../ports.hpp"
#include "../Constants.hpp"

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
     * Get the pose of the robot.
     * This is the result of odometer calculations.d
     * @return The pose of the robot.
     */
    Pose get_pose();

private:
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t left_drive_voltage = 0;
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t right_drive_voltage = 0;

    // Forward declaration of Odometry class
    class Odometry;

    // Pointer to calculate instance
    std::unique_ptr<Odometry> odometry;
  
    /** Power to set motors to from analog sticks. Will be between -127 and 127 */
    int32_t left_drive_power = 0;
    /** Power to set motors to from analog sticks. Will be between -127 and 127 */
    int32_t right_drive_power = 0;

    /** Type of drive control to use. */
    Constants::DriveType drive_type;

    // Motors:
    /** Smart pointer to the left, front motor. */
    std::unique_ptr<pros::Motor> left_front;
    /** Smart pointer to the right, front motor. */
    std::unique_ptr<pros::Motor> right_front;
    /** Smart pointer to the left, back motor. */
    std::unique_ptr<pros::Motor> left_back;
    /** Smart pointer to the right, back motor. */
    std::unique_ptr<pros::Motor> right_back;

    // Motor groups:
    /** Group of all motors on the left side of the robot. */
    std::unique_ptr<pros::MotorGroup> left_motors;
    /** Group of all motors on the right side of the robot. */
    std::unique_ptr<pros::MotorGroup> right_motors;

    /**
     * Get the position of the left and right motors in degrees.
     * @return A pair of the left and right motor positions in degrees. The first value is the left
     * motor position, and the second value is the right motor position. The values are averaged
     * between all motors on each side.
     */
    std::pair<double, double> get_position();

protected:
    /**
     * Constructor for Drivetrain subsystem.
     * Takes no arguments.
     * Can be called as: `Drivetrain drivetrain = Drivetrain();`
     */
    Drivetrain();
};

