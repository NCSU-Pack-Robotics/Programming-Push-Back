#include "LadyBrownLoadingLiftReverse.hpp"

void LadyBrownLoadingLiftReverse::initialize() {
    if (lady_brown.get_position() != LadyBrown::SCORE) {
        complete = true;
    }
    lift_start_degrees = lift.get_degrees();
}

void LadyBrownLoadingLiftReverse::periodic() {
    if (lift.get_degrees() - lift_start_degrees > -10) {
        lift.set_drive_power(-127);
    } else {
        complete = true;
        lift.set_drive_power(0);
    }
}

bool LadyBrownLoadingLiftReverse::is_complete() {
    return complete;
}

void LadyBrownLoadingLiftReverse::shutdown() {

}








