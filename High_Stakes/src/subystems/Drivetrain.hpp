#pragma once

#include <cstdint>

#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../ports.hpp"

/**
 * Enum for the type of drive control to use.
 * VOLTAGE: Set the voltage of the motors directly.
 * POWER: Set the power of the motors from analog sticks.
 */
enum DriveType {
        /** Set the voltage of the motors directly. */
        VOLTAGE,
        /** Set the power of the motors from analog sticks. */
        POWER
};

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
     * Set the voltage (in milli-volts) of the left and right motors.
     * @param left_mV The voltage to set the left motors to.
     * @param right_mV The voltage to set the right motors to.
     */
    void set_voltage(int32_t left_mV, int32_t right_mV);

    /**
     * Set the power (in Watts) of the left and right motors in range [-100, 100].
     * @param left_power The power to set the left motors to.
     * @param right_power The power to set the right motors to.
     */
    void set_drive_power(double left_power, double right_power);

    /**
     * Get the position of the left and right motors in degrees.
     * @return A pair of the left and right motor positions in degrees. The first value is the left
     * motor position, and the second value is the right motor position.
     */
    std::pair<double, double> get_position();

private:
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t left_drive_voltage = 0;
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t right_drive_voltage = 0;

    /** Power to set motors to from analog sticks. Will be between -127 and 127 */
    double left_drive_power = 0;
    /** Power to set motors to. Will be between -127 and 127 */
    double right_drive_power = 0;

    /** Type of drive control to use. */
    DriveType drive_type;

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

