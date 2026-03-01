#pragma once

#include "architecture/asset.hpp"
#include "subsystems/Drivetrain.hpp"
#include "architecture/Command.hpp"
#include "math/odometry/Pose.hpp"

/**
* The PurePursuit command is responsible for moving between two poses using the pure pursuit
* algorithm. The intent of this command is to be used series with several pure pursuit commands to
* move the robot through a series of waypoints.
*/
class PurePursuit : public Command {
    /** Is the PurePursuit command done executing? */
    bool done = false;

    /** An instance of the drivetrain subsystem. */
    Drivetrain &drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

    /** List of poses that make up the path. */
    std::vector<Pose> path;

    /** The last point in the path. This is necessary b/c points are added to the end of the path.
     * This will be the point that the robot will stop at. */
    Pose last_point;

    /** The distance between the robot and the lookahead point. */
    double lookahead;
    /** The distance from the end of the path to stop at. */
    double tolerance;
    /** The maximum power Pure Pursuit can use. */
    double max_speed;

public:
    /**
    * Constructor for the PurePursuit command.
    * @param path The path to follow.
    * @param lookahead The distance in inches for the lookahead distance
    * @param tolerance The number of inches from the end of the path to stop at.
    * @param max_speed The maximum power Pure Pursuit can apply to the drive motors.
    */
    explicit PurePursuit(const asset& path,
        double lookahead = Constants::PathFollowing::LOOKAHEAD_DISTANCE,
        double tolerance = Constants::PathFollowing::STOP_DISTANCE,
        double max_speed = 56);

    void initialize() override;

    void periodic() override;

    void shutdown() override;

    bool is_complete() override;
};
