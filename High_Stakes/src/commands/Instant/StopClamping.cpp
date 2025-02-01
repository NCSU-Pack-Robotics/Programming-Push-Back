#include "StopClamping.hpp"

#include "../../subystems/Clamp.hpp"

StopClamping::StopClamping() {
}

void StopClamping::execute() {
    Clamp& clamp = AbstractSubsystem::get_instance<Clamp>();

    clamp.set_enabled(false);
}