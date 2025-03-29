#pragma once
#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../ports.hpp"
#include "../Constants.hpp"

/** Class for the lift, which is a motor that moves a chain of upwards facing prongs designed to carry a plastic donut up the robot */
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

    /** Sets the lift to be braking. When it is in this state all methods that would usually move the lift will not work. When this method is called the lift is not
     * guaranteed to be braked, it will become braked when the next periodic loop runs if braking is still true.
     * @returns The previous brake state.
     */
    bool set_braking(bool braking);

    /**
     * Stops all motors immediately and sets powers/voltages to 0.
     */
    void brake_now();



    int32_t get_degrees();
private:
    /** The motor used for the lift */
    std::unique_ptr<pros::Motor> lift_motor;

    /** The voltage of the lift */
    int32_t lift_voltage = 0;

    /** The power of the lift */
    int32_t lift_power = 0;

    /** The current braking state of the lift */
    bool braking;

    /** Type of drive control to use. */
    Constants::DriveType drive_type;

protected:
    Lift();
};
