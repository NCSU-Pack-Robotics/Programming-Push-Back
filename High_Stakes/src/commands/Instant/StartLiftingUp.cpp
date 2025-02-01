#include "StartLiftingUp.hpp"

#include "../../Config.hpp"
#include "../../subystems/Lift.hpp"

StartLiftingUp::StartLiftingUp() {
}

void StartLiftingUp::execute() {
    Lift& lift = AbstractSubsystem::get_instance<Lift>();

    lift.set_drive_power(Constants::Controller::MotorSpeeds::LIFT_UP);
}