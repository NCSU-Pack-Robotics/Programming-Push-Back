#pragma once

#include <cstdint>

#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../ports.hpp"
#include "../Pose.hpp"

class Drivetrain : public AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

    /**
     * Set the voltage (in milli-volts) of the left and right motors.
     * @param leftVoltage The voltage (milli-volts) to set the left motors to.
     * @param rightVoltage The voltage (milli-volts) to set the right motors to.
     */
    void set_voltage(int32_t left_mV, int32_t right_mV);

    /**
     * Set the power of the left and right motors in range [-100, 100].
     * @param leftPower The power to set the left motors to.
     * @param rightPower The power to set the right motors to.
     */
    void set_drive_power(int32_t leftPower, int32_t rightPower);

    /**
     * Get the position of the left and right motors in degrees.
     * @return A pair of the left and right motor positions in degrees. The first value is the left
     * motor position, and the second value is the right motor position.
     */
    std::pair<double, double> get_position();

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

    /** Holds to current pose of the robot. */
    Pose pose;

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

protected:
    /**
     * Constructor for Drivetrain subsystem.
     * Takes no arguments.
     * Can be called as: `Drivetrain drivetrain = Drivetrain();`
     */
    Drivetrain();
};

