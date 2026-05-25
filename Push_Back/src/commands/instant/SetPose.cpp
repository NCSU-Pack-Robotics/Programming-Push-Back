#include "SetPose.hpp"

void SetPose::execute() {
    // Set the robot's pose to the new pose
    drivetrain.set_pose(this->new_pose);
}
