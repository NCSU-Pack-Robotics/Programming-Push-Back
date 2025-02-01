#include "StartClamping.hpp"

#include "../../subystems/Clamp.hpp"

StartClamping::StartClamping() {
}

void StartClamping::execute() {
    Clamp& clamp = AbstractSubsystem::get_instance<Clamp>();

    clamp.set_enabled(true);
}