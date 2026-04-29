#include "ToggleLift.hpp"

#include <subsystems/Lift.hpp>

void ToggleLift::execute()
{
    AbstractSubsystem::get_instance<Lift>().toggle();
}
