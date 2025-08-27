#include "StartIntakingOut.hpp"

#include "../../Config.hpp"
#include "../../subsystems/Intake.hpp"

StartIntakingOut::StartIntakingOut() {

}

void StartIntakingOut::execute() {
    Intake& intake = AbstractSubsystem::get_instance<Intake>();

    intake.set_drive_power(Constants::Controller::MotorSpeeds::INTAKE_OUTWARDS);
}

