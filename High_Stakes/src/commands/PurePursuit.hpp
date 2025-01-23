#pragma once

#include "Drivetrain.hpp"
#include "../Command.hpp"
#include "../Pose.hpp"

/**
* The PurePursuit command is responsible for moving between two poses using the pure pursuit
* algorithm. The intent of this command is to be used series with several pure pursuit commands to
* move the robot through a series of waypoints.
*/
class PurePursuit : public Command {
    /** An instance of the drivetrain subsystem. */
    Drivetrain &drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

    /** The starting pose of the robot. */
    Pose start;
    /** The goal pose of the robot. */
    Pose end;
    /** The pose of the robot as it follows the path. */
    Pose current;

public:
    /**
    * Constructor for the PurePursuit command.
    * @param start The starting pose of the robot.
    * @param end The ending pose of the robot.
    */
    PurePursuit(Pose start, Pose end);

    void initialize() override;

    void periodic() override;

    void shutdown() override;

    bool is_complete() override;
};
