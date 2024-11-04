#pragma once
#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../Constants.hpp"


class Intake : public AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

    /**
     * Set the voltage (in milli-volts) of the intake in range [-12000,12000].
     * @param voltage The voltage in mV to set the intake to.
     */
    void set_voltage(int32_t voltage);

    /**
     * Set the power of the intake in range [-127, 127].
     * @param power The power to set the intake to.
     */
    void set_drive_power(int32_t power);
private:
    /** The motor used for the lift */
    std::unique_ptr<pros::Motor> intake_motor;

    /** The voltage of the intake */
    int32_t intake_voltage = 0;

    /** The power of the intake */
    int32_t intake_power = 0;

    /** Type of drive control to use. */
    Constants::DriveType drive_type;

protected:
    Intake();

};