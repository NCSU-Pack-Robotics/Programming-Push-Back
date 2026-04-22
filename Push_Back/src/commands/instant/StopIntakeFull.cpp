#include "StopIntakeFull.hpp"

#include <subsystems/Intakes.hpp>
#include <Config.hpp>

void StopIntakeFull::execute() {
    IntakeBottom& intake_bottom = AbstractSubsystem::get_instance<IntakeBottom>();
    IntakeTop& intake_top = AbstractSubsystem::get_instance<IntakeTop>();

    intake_bottom.set_drive_power(0);
    intake_top.set_drive_power(0);
}
