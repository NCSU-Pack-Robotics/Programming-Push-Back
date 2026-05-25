#include "StopColorSorting.hpp"

#include <subsystems/Intakes.hpp>

void StopColorSorting::execute() {
    IntakeTop& intake_top = AbstractSubsystem::get_instance<IntakeTop>();

    intake_top.set_drive_power(0);
}
