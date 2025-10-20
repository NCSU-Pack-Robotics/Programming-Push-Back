#pragma once

#include "../include/main.h"
#include "Config.hpp"
#include "Constants.hpp"
#include <vector>
#include <memory>


/**
 * The framework used for all motor subsystems
 * Each motor subsystem runs every loop to control a specific thing
 * For example: the drivetrain subsystem controls movement by updating the power in each motor every loop
 */
class AbstractMotorSubsystem {
public:
    virtual ~AbstractMotorSubsystem() = default;

    /** Runs every loop (when robot is not disabled)  */
    virtual void move();

    /**
     * Runs every loop (when robot is disabled)
     * <code>shutdown()</code> is called before looping over this method.
     */
    virtual void disabled_move();

    /**
     * Runs when the robot shuts down.
     * This is called before looping over <code>disabled_periodic()</code> when the robot is
     * disabled.
     */
    virtual void shutdown();

    /**
    * Set the power of the motor in range [-127, 127].
    * @param power The power to set all motors to.
    */
    virtual void set_power(int32_t power);

    /**
     * Set the voltage (in milli-volts) of motor in range [-12000,12000].
     * @param voltage The voltage to set all motors to.
    */
    virtual void set_voltage(int32_t voltage);

    /** Sets all the motors to be braking. When it is in this state all methods that would usually move the lift will not work. When this method is called the lift is not
     * guaranteed to be braked, it will become braked when the next periodic loop runs if braking is still true.
     * @returns The previous brake state.
     */
    bool set_braking(bool braking);

    /**
     * Stops all motors immediately and sets powers/voltages to 0.
     */
    void brake_now();

protected:
    /**
     * Constructor for AbstractMotorSubsystem.
     * Takes no arguments and is protected to ensure only subclasses can instantiate.
     */
    AbstractMotorSubsystem() = default;

    /** Vector of motor objects used for the subsystem */
    std::vector<std::shared_ptr<pros::Motor>> motors;

    /** The power of all the motors. */
    int32_t drive_power = 0;

    /** The voltage of all the motors */
    int32_t drive_voltage = 0;

    /** The current braking state of the motors. */
    bool braking = false;

    /** Type of drive control to use. */
    Constants::DriveType drive_type = Constants::DriveType::VOLTAGE;
};
