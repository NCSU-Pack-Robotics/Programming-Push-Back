#pragma once

#include "../../Command.hpp"
#include "../../Pose.hpp"
#include "../../subystems/Drivetrain.hpp"

/**
 * This command sets the pose of the robot to a specified location.
 * This command should be used for resetting the robot's position when we know the position for sure.
 * For example, after the robot hits a wall to align itself.
 */
class SetPose : public InstantCommand {
    /** The pose to set the robot to. */
    Pose new_pose;

    /** An instance of the Drivetrain class to set the pose of */
    Drivetrain &drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

public:
    /**
     * Constructor for SetPose command.
     * @param new_pose The new pose to set the robot to.
     */
    SetPose(Pose new_pose);

    void execute() override;
};

