#pragma once

#include "../asset.hpp"
#include "../subystems/Drivetrain.hpp"
#include "../Command.hpp"
#include "../Pose.hpp"

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

    /** The pose of the robot as it follows the path. */
    Pose current;

    /** List of poses that make up the path. */
    std::vector<Pose> path;

    /** The distance between the robot and the lookahead point. */
    double lookahead;

public:
    /**
    * Constructor for the PurePursuit command.
    * @param path The path to follow.
    * @param lookahead The distance in inches for the lookahead distance
    */
    explicit PurePursuit(const asset& path, double lookahead = Constants::PathFollowing::LOOKAHEAD_DISTANCE);

    void initialize() override;

    void periodic() override;

    void shutdown() override;

    bool is_complete() override;
};
