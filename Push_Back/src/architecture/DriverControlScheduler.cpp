#include "DriverControlScheduler.hpp"

#include "ButtonMappings.hpp"

DriverControlScheduler::DriverControlScheduler() : ParallelCommand({}){
}

bool DriverControlScheduler::is_complete() {
    return false;
}

void DriverControlScheduler::initialize() {
    // TODO: remove when better system is implemented, see issue #29
    drivetrain.set_reversing(false);
}

int32_t scale_power(const int32_t power, double scaling_factor) {
    // https://www.desmos.com/calculator/arejdbcye3
    scaling_factor = std::clamp(scaling_factor, 0.0, 400.0);
    int32_t scaled_power = std::abs(power) * std::exp((scaling_factor / 100 * std::log(127) / 127) * (std::abs(power) - 127));
    // Needed because exponential function cannot produce negative numbers
    return power < 0 ? -scaled_power : scaled_power;
}

int32_t cutoff_power(const int32_t power, int32_t cutoff)
{
    return abs(power) > cutoff ? power : 0;
}

void DriverControlScheduler::periodic() {
#if THINK
    // Normal controls
    double x = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    double y = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    double r = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
    // TODO: Test how this feels
    // double r = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) / 127.0 * 0.6;

    int32_t fl = y + x + r;
    int32_t fr = y - x - r;
    int32_t bl = y - x + r;
    int32_t br = y + x - r;
#endif

#if DO
    // Garret's controls
    double left_x = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
    double left_y = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    double right_x = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    double right_y = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

    int32_t fl = left_y + left_x;
    int32_t bl = left_y - left_x;
    int32_t fr = right_y - left_x;
    int32_t br = right_y + left_x;
#endif

    int32_t scaled_fl = cutoff_power(scale_power(fl, Constants::Controller::INPUT_SCALING_FACTOR), Constants::Controller::INPUT_CUTOFF_AMOUNT);
    int32_t scaled_fr = cutoff_power(scale_power(fr, Constants::Controller::INPUT_SCALING_FACTOR), Constants::Controller::INPUT_CUTOFF_AMOUNT);
    int32_t scaled_bl = cutoff_power(scale_power(bl, Constants::Controller::INPUT_SCALING_FACTOR), Constants::Controller::INPUT_CUTOFF_AMOUNT);
    int32_t scaled_br = cutoff_power(scale_power(br, Constants::Controller::INPUT_SCALING_FACTOR), Constants::Controller::INPUT_CUTOFF_AMOUNT);

    drivetrain.set_drive_power(scaled_fl, scaled_fr, scaled_br, scaled_bl);

    for (auto &[button, command] : BINDS) {
        // controller state for this tick
        const bool new_controller_state = controller.get_digital(button);

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