#include "DriverControlScheduler.hpp"
#include "AbstractSubsystem.hpp"
#include "Config.hpp"

bool DriverControlScheduler::is_complete() {
    return false;
}

void DriverControlScheduler::initialize() {
    controller = pros::Controller{pros::E_CONTROLLER_MASTER};
}

void DriverControlScheduler::periodic() {
    for (auto &[button, command] : Constants::Controller::BINDS) {
        // If a bind exists for that button and the buttons pressed, then add the command to the queue
        if (command.has_value() && controller.get_digital(button)) {
            this->add_command(command.value()());
        }
    }
}

void DriverControlScheduler::shutdown() {
    // Does nothing
}


