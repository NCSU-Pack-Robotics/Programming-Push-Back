#include "LadyBrown.hpp"

#include "../ports.hpp"

LadyBrown::LadyBrown() {

}

void LadyBrown::initialize() {
    lb_motor = std::make_unique<pros::Motor>(Ports::LADYBROWN_MOTOR_PORT, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
    lb_sensor = std::make_unique<pros::Rotation>(Ports::LADYBROWN_ROTATION_PORT);

    lb_motor->tare_position();
    lb_motor->move_velocity(0);

    position = Constants::LadyBrown::Position::STOW;
    position_index = 0;
}

void LadyBrown::periodic() {
    // update the ladybrown to be in the correct position

    int32_t target_rotation = Constants::LadyBrown::Positions[position_index].second;
    int32_t voltage = lb_pid.calculate(abs(target_rotation - lb_sensor->get_angle()));
    lb_motor->move_voltage(voltage);

    // never go below 0 or over 215
}

void LadyBrown::shutdown() {

}

void LadyBrown::disabled_periodic() {

}

void LadyBrown::next_position() {
    // update to next position regardless if the motor has finished getting to the current position or not
    if (position_index == Constants::LadyBrown::Positions.size() - 1) {
        position_index = 0;
    } else {
        position_index++;
    }
    position = Constants::LadyBrown::Positions[position_index].first;
}
