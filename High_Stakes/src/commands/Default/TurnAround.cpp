#include "TurnAround.hpp"


void TurnAround::initialize() {
    starting_heading = drivetrain.get_pose().heading;
    target_heading = starting_heading + M_PI;
    printf("starting heading: %f", starting_heading);
    drivetrain.set_braking(false);
}

void TurnAround::periodic() {
    // double output = heading_pid.calculate(target_heading - drivetrain->get_pose().heading);
    // printf("heading: %f\n", drivetrain->get_pose().heading);
    // printf("output: %f\n", output);
    drivetrain.set_voltage(-12000, 12000);
}

bool TurnAround::is_complete() {
    printf("difference: %f\n", fabs(drivetrain.get_pose().heading - starting_heading));
    if (target_heading - drivetrain.get_pose().heading < 0.65) {
        printf("returning");
        drivetrain.set_braking(true);
        return true;
    }
    return false;
}

void TurnAround::shutdown() {
    printf("shutting down");
}

