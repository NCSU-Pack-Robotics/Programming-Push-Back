#include "StopLifting.hpp"

#include "Lift.hpp"

StopLifting::StopLifting() {
}

void StopLifting::execute() {
    Lift& lift = AbstractSubsystem::get_instance<Lift>();

    lift.set_drive_power(0);
}