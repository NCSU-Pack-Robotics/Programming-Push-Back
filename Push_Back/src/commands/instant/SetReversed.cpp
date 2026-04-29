#include "SetReversed.hpp"

#include "subsystems/Drivetrain.hpp"

void SetReversed::execute() {
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

    drivetrain.set_reversing(this->reversed);
}
