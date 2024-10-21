#pragma once

#include <cstdint>

#include "../../include/main.h"
#include "../Subsystem.hpp"
#include "../ports.hpp"

class Drivetrain : public SubsystemAbstract {
    friend class SubsystemAbstract;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

private:
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t leftDrivePower = 0;
    /** Voltage in mV to set motors to. Will be between -12,000 and +12,000. */
    int32_t rightDrivePower = 0;

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

