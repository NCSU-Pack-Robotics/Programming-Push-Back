#include "StartColorSorting.hpp"

#include <subsystems/Intakes.hpp>
#include <Config.hpp>

void StartColorSorting::execute() {
    IntakeTop& intake_top = AbstractSubsystem::get_instance<IntakeTop>();

    intake_top.set_drive_power(-Constants::Controller::MotorSpeeds::INTAKE_INWARDS);
}
