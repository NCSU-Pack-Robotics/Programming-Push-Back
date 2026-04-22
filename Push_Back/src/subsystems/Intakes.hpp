#pragma once

#include <Constants.hpp>
#include <cstdint>
#include <motors.hpp>
#include "ports.hpp"
#include "architecture/AbstractSubsystem.hpp"
// two motors here
// one command turns bottom and top motor on
// other command enables color sorting (reverses top motor)


template <std::int8_t PORT>
class Intakes : public AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override
    {
        intake_motor = std::make_unique<pros::Motor>(PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
        intake_motor->tare_position();
        intake_motor->move_velocity(0);
        braking = false;
    }

    void periodic() override
    {
        if (braking) {
            brake_now();
            // return because nothing after this matters if it is braking.
            return;
        } else {
            intake_motor->set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
        }

        switch (drive_type) {
            case Constants::DriveType::POWER: {
                intake_motor->move(intake_power);
                break;
            }
            case Constants::DriveType::VOLTAGE: {
                intake_motor->move_voltage(intake_voltage);
                break;
            }
        }
    }

    void disabled_periodic() override
    {

    }

    void shutdown() override
    {
        brake_now();
    }

    /**
     * Set the voltage (in milli-volts) of the intake in range [-12000,12000].
     * @param voltage The voltage in mV to set the intake to.
     */
    void set_voltage(int32_t voltage)
    {
        intake_voltage = std::clamp(voltage, INT32_C(-12000), INT32_C(12000));

        drive_type = Constants::DriveType::VOLTAGE;
    }

    /**
     * Set the power of the intake in range [-127, 127].
     * @param power The power to set the intake to.
     */
    void set_drive_power(int32_t power)
    {
        intake_power = std::clamp(power, INT32_C(-127), INT32_C(127));

        drive_type = Constants::DriveType::POWER;
    }

    /** Sets the intake to be braking. When it is in this state all methods that would usually move the intake will not work. When this method is called the intake is not
     * guaranteed to be braked, it will become braked when the next periodic loop runs if braking is still true.
     * @returns The previous brake state.
     */
    bool set_braking(bool braking)
    {
        // Don't need to do anything if its already in the right mode
        if (braking == this->braking) return braking;
        bool old = this->braking;

        this->braking = braking;
        return old;
    }

    /**
     * Immediately stop the intake, setting all power and voltage power to 0.
     */
    void brake_now()
    {
        intake_motor->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
        intake_voltage = 0;
        intake_power = 0;
        intake_motor->brake();
    }

private:
    /** The motor used for the lift */
    std::unique_ptr<pros::Motor> intake_motor;

    /** The voltage of the intake */
    int32_t intake_voltage = 0;

    /** The power of the intake */
    int32_t intake_power = 0;

    /** Type of drive control to use. */
    Constants::DriveType drive_type = Constants::DriveType::VOLTAGE;

    /** The current braking state of the intake */
    bool braking;

protected:
    Intakes() = default;
};


using IntakeBottom = Intakes<Ports::INTAKE_BOTTOM_MOTOR_PORT>;
using IntakeTop = Intakes<Ports::INTAKE_TOP_MOTOR_PORT>;
using IntakeEnd = Intakes<Ports::INTAKE_END_MOTOR_PORT>;
