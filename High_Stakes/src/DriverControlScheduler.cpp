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

    for (auto &[button, command] : Constants::Controller::BINDS) {
        // controller state for this tick
        bool new_controller_state = controller.get_digital(button);

        if (command[0].has_value() && controller.get_digital_new_press(button)) { // button just pressed
            this->add_command(command[0].value()());
        } else if (command[1].has_value() && controller.get_digital(button)) { // button down but not just pressed
            this->add_command(command[1].value()());
        } else if (command[2].has_value() && !new_controller_state && controller_state[button]) { // if the button isn't pressed this tick and it was pressed last tick
            this->add_command(command[2].value()());
        }

        // update actual map
        controller_state[button] = new_controller_state;
    }
    ParallelCommand::periodic();
}

void DriverControlScheduler::shutdown() {
    // Does nothing
}


