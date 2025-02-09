#include "StopIntaking.hpp"

#include "../../Config.hpp"
#include "../../subystems/Intake.hpp"

StopIntaking::StopIntaking() {
}

void StopIntaking::execute() {
    Intake& intake = AbstractSubsystem::get_instance<Intake>();

    intake.set_drive_power(0);
}
