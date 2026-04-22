#include "StopIntakeEnd.hpp"

#include <subsystems/Intakes.hpp>

void StopIntakeEnd::execute() {
    IntakeEnd& intake = AbstractSubsystem::get_instance<IntakeEnd>();

    intake.set_drive_power(0);
}
