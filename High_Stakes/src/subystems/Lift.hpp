#pragma once
#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../ports.hpp"


class Lift : public AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

    /**
     * Set the voltage (in milli-volts) of the lift in range [-12000,12000].
     * @param voltage The voltage in mV to set the lift to.
     */
    void set_voltage(int32_t voltage);

    /**
     * Set the power of the lift in range [-127, 127].
     * @param power The power to set the lift to.
     */
    void set_drive_power(int32_t power);
private:
    /** The motor used for the lift */
    std::unique_ptr<pros::Motor> lift_motor;

    /** The voltage of the lift */
    int32_t lift_voltage = 0;

    /** The power of the lift */
    int32_t lift_power = 0;

    /** Type of drive control to use. */
    DriveType drive_type;

protected:
    Lift();

};
