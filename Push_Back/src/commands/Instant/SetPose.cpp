#include "SetPose.hpp"

SetPose::SetPose(Pose new_pose) : new_pose(new_pose) {}

void SetPose::execute() {
    // Set the robot's pose to the new pose
    drivetrain.set_pose(this->new_pose);
}
