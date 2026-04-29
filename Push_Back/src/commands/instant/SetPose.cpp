#include "SetPose.hpp"

SetPose::SetPose(const Pose& new_pose) : new_pose(new_pose) {}

void SetPose::execute() {
    // Set the robot's pose to the new pose
    drivetrain.set_pose(this->new_pose);
}

std::string SetPose::to_string() const {
    return "SetPose((" + std::to_string(new_pose.x) + ", " + std::to_string(new_pose.y) + 
           ", " + std::to_string(new_pose.heading) + "))";
}