#include "StartIntakeFull.hpp"

#include <subsystems/Intakes.hpp>
#include <Config.hpp>

void StartIntakeFull::execute() {
    IntakeBottom& intake_bottom = AbstractSubsystem::get_instance<IntakeBottom>();
    IntakeTop& intake_top = AbstractSubsystem::get_instance<IntakeTop>();

    intake_bottom.set_drive_power(Constants::Controller::MotorSpeeds::INTAKE_INWARDS);
    intake_top.set_drive_power(Constants::Controller::MotorSpeeds::INTAKE_INWARDS);
}
