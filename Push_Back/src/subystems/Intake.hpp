#pragma once
#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../Constants.hpp"

/** Class for the intake, which uses motors to spin two wheels inwards, designed to suck a plastic donut into the lift system */
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

    /** Sets the intake to be braking. When it is in this state all methods that would usually move the intake will not work. When this method is called the intake is not
     * guaranteed to be braked, it will become braked when the next periodic loop runs if braking is still true.
     * @returns The previous brake state.
     */
    bool set_braking(bool braking);

    /**
     * Immediately stop the intake, setting all power and voltage power to 0.
     */
    void brake_now();

private:
    /** The motor used for the lift */
    std::unique_ptr<pros::Motor> intake_motor;

    /** The voltage of the intake */
    int32_t intake_voltage = 0;

    /** The power of the intake */
    int32_t intake_power = 0;

    /** Type of drive control to use. */
    Constants::DriveType drive_type;

    /** The current braking state of the intake */
    bool braking;

protected:
    Intake();

};