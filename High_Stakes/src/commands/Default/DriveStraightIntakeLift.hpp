#pragma once

#include "../DriveStraight.hpp"
#include "../../subystems/Intake.hpp"
#include "../../subystems/Lift.hpp"

/**
 * This command uses the <code>DriveStraight</code> command to drive the robot straight while
 * and then this command adds lifting and intaking at the same time. On shutdown, this command
 * will stop the robot, lift, and intake.
 */
class DriveStraightIntakeLift : public DriveStraight {
    /** The lift subsystem */
    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    /** The intake subsystem */
    Intake& intake = AbstractSubsystem::get_instance<Intake>();

public:
    /**
     * Constructor for the DriveStraightIntakeLift command.
     * @param inches The number of inches to drive. May be negative to drive backwards.
     * @param tolerance How close (in inches) the robot needs to be to the target distance to be
     * considered done.
     * @param max_time The maximum time (in seconds) this command is allowed to run. If the command
     * exceeds this time, it will be considered done. Default does not use this.
     */
    DriveStraightIntakeLift(double inches, double tolerance,
        double max_time = std::numeric_limits<double>::max());

    void initialize() override;

    void shutdown() override;
};