#include "DriverControlScheduler.hpp"

#include "ButtonMappings.hpp"
#include "Config.hpp"

DriverControlScheduler::DriverControlScheduler() : ParallelCommand({}){
}

bool DriverControlScheduler::is_complete() {
    return false;
}

void DriverControlScheduler::initialize() {
    // TODO: remove when better system is implemented, see issue #29
    drivetrain.set_reversing(false);
}

int32_t DriverControlScheduler::scale_power(const int32_t power, double scaling_factor) const {
    // https://www.desmos.com/calculator/arejdbcye3
    scaling_factor = std::clamp(scaling_factor, 0.0, 400.0);
    int32_t scaled_power = std::abs(power) * std::exp((scaling_factor / 100 * std::log(127) / 127) * (std::abs(power) - 127));
    // Needed because exponential function cannot produce negative numbers
    return power < 0 ? -scaled_power : scaled_power;
}

void DriverControlScheduler::periodic() {

    // double x = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) / 127.0;
    // double y = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / 127.0;
    // double r = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) / 127.0;
    // // double r = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) / 127.0 * 0.6;
    //
    // double fl = y + x + r;
    // double fr = y - x - r;
    // double bl = y - x + r;
    // double br = y + x - r;
    //
    //
    // double maxMag = std::max({fabs(fl), fabs(fr), fabs(bl), fabs(br)});
    // if (maxMag > 1.0) {
    //     fl /= maxMag;
    //     fr /= maxMag;
    //     bl /= maxMag;
    //     br /= maxMag;
    // }
    //
    // drivetrain.set_drive_power(static_cast<int32_t>(fl) * 127, static_cast<int32_t>(fr) * 127, static_cast<int32_t>(br) * 127, static_cast<int32_t>(bl) * 127);

    double left_x = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) / 127.0;
    double left_y = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127.0;
    double right_x = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) / 127.0;
    double right_y = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / 127.0;

    // double r = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) / 127.0;
    // double r = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) / 127.0 * 0.6;

    double fl = left_y + left_x;
    double bl = left_y - left_x;
    double fr = right_y - left_x;
    double br = right_y + left_x;

    drivetrain.set_drive_power(static_cast<int32_t>(fl) * 127, static_cast<int32_t>(fr) * 127, static_cast<int32_t>(br) * 127, static_cast<int32_t>(bl) * 127);


    // double maxMag = std::max({fabs(fl), fabs(fr), fabs(bl), fabs(br)});
    // if (maxMag > 1.0) {
    //     fl /= maxMag;
    //     fr /= maxMag;
    //     bl /= maxMag;
    //     br /= maxMag;
    // }





    // const int32_t left_power_scaled = scale_power(left_power, Constants::Controller::INPUT_SCALING_FACTOR);
    // const int32_t right_power_scaled = scale_power(right_power, Constants::Controller::INPUT_SCALING_FACTOR);
    //
    // if (abs(left_power_scaled) > 1 || abs(right_power_scaled) > 1) {
    //     drivetrain.set_drive_power(left_power_scaled, right_power_scaled);
    //     drivetrain.set_braking(false);
    // } else {
    //     drivetrain.set_drive_power(0,0);
    // }

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