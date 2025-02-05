#include "TurnAround.hpp"
#include "../../subystems/Drivetrain.hpp"

void TurnAround::initialize() {
    drivetrain = &AbstractSubsystem::get_instance<Drivetrain>();
    starting_heading = drivetrain->get_pose().heading;
    printf("starting heading: %f", starting_heading);
}

void TurnAround::periodic() {
    double output = heading_pid.calculate(M_PI - fabs(drivetrain->get_pose().heading - starting_heading));
    printf("heading: %f\n", drivetrain->get_pose().heading);
    printf("output: %f\n", output);
    drivetrain->set_voltage(output, -output);
}

bool TurnAround::is_complete() {
    printf("difference: %f\n", fabs(drivetrain->get_pose().heading - starting_heading));
    if (fabs(drivetrain->get_pose().heading - starting_heading) >= M_PI) {
        printf("returning");
        drivetrain->brake();
        return true;
    }
    return false;
}

void TurnAround::shutdown() {
    printf("shutting down");
}

