#include "TurnAround.hpp"

void TurnAround::initialize() {
    // starting_heading = drivetrain.get_pose().heading;
}

void TurnAround::periodic() {
    // drivetrain.set_drive_power(127, -127);
}

bool TurnAround::is_complete() {
    // if (starting_heading - drivetrain.get_pose().heading >= 180) {
    //     drivetrain.brake();
    //     return true;
    // }
    // return false;
}

void TurnAround::shutdown() {

}

