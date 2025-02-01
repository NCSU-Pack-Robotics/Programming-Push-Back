#include "StartClamping.hpp"

#include "../Config.hpp"

StartClamping::StartClamping() {
}

void StartClamping::execute() {
    Intake& intake = AbstractSubsystem::get_instance<Intake>();

    intake.set_drive_power(Constants::Controller::MotorSpeeds::INTAKE_INWARDS);
}