#include "StartIntaking.hpp"

#include "../Config.hpp"
#include "../subystems/Intake.hpp"

void StartIntaking::execute() {
    Intake& intake = AbstractSubsystem::get_instance<Intake>();

    intake.set_drive_power(Constants::Controller::MotorSpeeds::INTAKE_INWARDS);
}
