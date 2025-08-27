#include "ToggleClamp.hpp"

#include "../../subsystems/Clamp.hpp"

ToggleClamp::ToggleClamp() {
}

void ToggleClamp::execute() {
    Clamp& clamp = AbstractSubsystem::get_instance<Clamp>();

    clamp.toggle();
}