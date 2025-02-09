#include "LadyBrown.hpp"

#include "../ports.hpp"

LadyBrown::LadyBrown() {

}

void LadyBrown::initialize() {
    lb_motor = std::make_unique<pros::Motor>(Ports::LADYBROWN_MOTOR_PORT, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
    lb_sensor = std::make_unique<pros::Rotation>(Ports::LADYBROWN_ROTATION_PORT);

    lb_motor->tare_position();
    lb_motor->move_velocity(0);

    position = STOW;
    position_index = 0;
}

void LadyBrown::periodic() {
    // update the ladybrown to be in the correct position


    int32_t curr_angle = lb_sensor->get_angle();

    // return;

    // prevent the angle from being read as 359 deg and trying to go backwards.
    if (curr_angle > 35900 && curr_angle < 36000) {
        curr_angle = 0;
    }

    const int32_t target_rotation = Positions[position_index].second;
    const int32_t voltage = lb_pid.calculate(target_rotation - curr_angle);
    lb_motor->move_voltage(voltage);

    // never go below 0 or over 215

}

void LadyBrown::shutdown() {
    // Just stop the motor
    lb_motor-> move_velocity(0);
}

void LadyBrown::disabled_periodic() {

}

void LadyBrown::next_position() {
    // update to next position regardless if the motor has finished getting to the current position or not
    if (position_index == Positions.size() - 1) {
        position_index = 0;
    } else {
        position_index++;
    }
    position = Positions[position_index].first;
}

LadyBrown::Position LadyBrown::get_position() const {
    return position;
}
