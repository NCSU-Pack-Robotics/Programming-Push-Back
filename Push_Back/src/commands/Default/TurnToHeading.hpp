#pragma once

#include "../../Command.hpp"
#include "../../subsystems/Drivetrain.hpp"
#include "../../Timer.hpp"

/**
 * This command turns the robot to a specified heading using a point turn.
 * It uses PID and requires the constants to be tuned in the config.
*/
class TurnToHeading : public Command {
    /** How close the robot can be to the target heading before finishing the command (radians) */
    double tolerance;

    /** The PID controller used to control the robot's heading */
    PID controller;

    /* Below are the controllers that are averaged in the initialize method
     * to get the best of all worlds */
    // PID controller = PID(6000, 1200, 120);  // Used for turning PI radians
    // PID controller = PID(8000, 0, 0);  // Used for turning PI/4 radians

protected:
    /** The heading to which the robot will turn to (radians) */
    double target_heading;

    /** An instance of the drivetrain class to control the robot's movement */
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

    Timer timer;
    double max_time;

public:
    /**
     * Constructs a <code>TurnToHeading</code> command.
     * @param target_heading The heading to which the robot will turn to (radians)
     * @param tolerance How close the robot can be to the target heading before finishing the
     * command (radians)
     * @param max_time The maximum time the command will run (seconds). Defaults to the maximum
     value for a double.
     */
    TurnToHeading(double target_heading, double tolerance, double max_time = std::numeric_limits<double>::max());

    bool is_complete() override;

    void initialize() override;

    void periodic() override;

    void shutdown() override;
};