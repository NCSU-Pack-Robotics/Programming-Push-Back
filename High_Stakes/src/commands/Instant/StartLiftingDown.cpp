#include "StartLiftingDown.hpp"

#include "../../Config.hpp"
#include "../../subystems/Lift.hpp"
#include "../../subystems/LadyBrown.hpp"

StartLiftingDown::StartLiftingDown() {
}

void StartLiftingDown::execute() {
    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    LadyBrown& lady_brown = AbstractSubsystem::get_instance<LadyBrown>();
    int32_t speed = Constants::Controller::MotorSpeeds::LIFT_DOWN;
    // If ladybrown is in LOAD position, run at a modified speed.
    if (lady_brown.get_position() == LadyBrown::Position::LOAD) {
        speed *= Constants::Controller::MotorSpeeds::LIFT_LADYBROWN_SPEED_MULTIPLIER;
    }
    lift.set_drive_power(speed);
}