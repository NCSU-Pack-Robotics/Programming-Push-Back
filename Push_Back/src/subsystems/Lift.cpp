#include "Lift.hpp"

void Lift::initialize()
{

}

void Lift::toggle()
{
    enabled = !enabled;
    lift_piston.set_value(enabled);
}

void Lift::periodic()
{

}

void Lift::disabled_periodic()
{

}

void Lift::shutdown()
{

}
