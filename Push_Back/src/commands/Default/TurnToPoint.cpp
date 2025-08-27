#include "TurnToPoint.hpp"

TurnToPoint::TurnToPoint(const double x, const double y, const double tolerance) :
    TurnToHeading(0, tolerance), x(x), y(y) {
    // Additional initialization if needed
}

void TurnToPoint::initialize() {
    // Get the current pose of the robot
    const Pose& pose = this->drivetrain.get_pose();

    // Calculate the angle to the target point
    const double angle_to_target = atan2(y - pose.y, x - pose.x);

    // Set the target heading to the angle to the target point
    this->target_heading = angle_to_target;

    // Call the base class initialize method
    TurnToHeading::initialize();
}
