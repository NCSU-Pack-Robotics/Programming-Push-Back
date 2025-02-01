#include "StartLiftingDown.hpp"

#include "../Config.hpp"
#include "Lift.hpp"

StartLiftingDown::StartLiftingDown() {
}

void StartLiftingDown::execute() {
    Lift& lift = AbstractSubsystem::get_instance<Lift>();

    lift.set_drive_power(Constants::Controller::MotorSpeeds::LIFT_DOWN);
}