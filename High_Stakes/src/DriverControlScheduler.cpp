#include "DriverControlScheduler.hpp"
#include "AbstractSubsystem.hpp"

bool DriverControlScheduler::is_complete() {
    return false;
}

void DriverControlScheduler::initialize() {
    controller = AbstractSubsystem::get_instance<pros::Controller>();
}

void DriverControlScheduler::periodic() {

}

void DriverControlScheduler::shutdown() {
    // Does nothing
}


