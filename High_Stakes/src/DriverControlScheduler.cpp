#include "DriverControlScheduler.hpp"
#include "Config.hpp"
#include "subystems/Intake.hpp"

DriverControlScheduler::DriverControlScheduler() : ParallelCommand({}){
}

bool DriverControlScheduler::is_complete() {
    return false;
}

void DriverControlScheduler::initialize() {

}

void DriverControlScheduler::periodic() {
    // Set drive power from right joystick using arcade drive
    int32_t left_power = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) + controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    int32_t right_power = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) - controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    drivetrain.set_drive_power(left_power, right_power);

    // TODO: Think of some way to set subsystems power to 0 if the button isn't pressed
    AbstractSubsystem::get_instance<Intake>().set_drive_power(0);

    for (auto &[button, command] : Constants::Controller::BINDS) {
        // If a bind exists for that button and the buttons pressed, then add the command to the queue
        if (command.has_value() && controller.get_digital(button)) {
            this->add_command(command.value()());
        }
    }
    ParallelCommand::periodic();
}

void DriverControlScheduler::shutdown() {
    // Does nothing
}


