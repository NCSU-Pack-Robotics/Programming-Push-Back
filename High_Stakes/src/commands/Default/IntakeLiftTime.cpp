//
// Created by koray on 5/7/2025.
//

#include "IntakeLiftTime.hpp"

IntakeLiftTime::IntakeLiftTime(const double time,
    const double intake_speed,
    const double lift_speed,
    const double direction) : ParallelCommand{}{

    add_command(std::make_unique<LiftTime>(time, direction, lift_speed));
    add_command(std::make_unique<IntakeTime>(time, direction, lift_speed));
}

void IntakeLiftTime::initialize() {
}

void IntakeLiftTime::shutdown() {
}
