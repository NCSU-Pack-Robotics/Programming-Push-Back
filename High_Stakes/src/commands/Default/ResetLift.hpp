#pragma once
#include "../../Command.hpp"

#include "../../subystems/HookSensor.hpp"
#include "../../subystems/Lift.hpp"
/** This command continues move the lift in the upwards direction until
 * the hook sensor is covered by the hook, meaning it has been reset.
 */
class ResetLift : public Command {
    void initialize() override;
    void periodic() override;
    void shutdown() override;
    bool is_complete() override;

private:
    /** A reference to the lift motor */
    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    /** A reference to the hook sensor */
    HookSensor& hook_sensor = AbstractSubsystem::get_instance<HookSensor>();
    /** A bool that is true if the sensor has already detected the lift has passed */
    bool has_passed;
};