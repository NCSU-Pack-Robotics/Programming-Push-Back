#include "StartIntakeEnd.hpp"

#include <subsystems/Intakes.hpp>
#include <Config.hpp>

void StartIntakeEnd::execute() {
    IntakeEnd& intake = AbstractSubsystem::get_instance<IntakeEnd>();

    intake.set_drive_power(Constants::Controller::MotorSpeeds::INTAKE_INWARDS);
}
