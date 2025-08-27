#pragma once

#include "../asset.hpp"
#include "../Command.hpp"
#include "../subsystems/Intake.hpp"
#include "../subsystems/Lift.hpp"
#include "../subsystems/Drivetrain.hpp"

/**
 * Used to follow path, intake a ring, and lift the ring onto a mobile goal simultaneously.
 */
class PPIntakeLift : public ParallelCommand {
    /** Reference to drivetrain subsystem. */
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

public:
    /**
     * Constructor that sets the path to follow.
     * @param path The pah to follow.
     * @param lookahead The distance in inches for the lookahead distance
     * @param tolerance The number of inches from the end of the path to stop at.
     * @param max_speed The maximum power Pure Pursuit can apply to the drive motors.
     */
    PPIntakeLift(asset path,
        double lookahead = Constants::PathFollowing::LOOKAHEAD_DISTANCE,
        double tolerance = Constants::PathFollowing::STOP_DISTANCE,
        double max_speed = 56);

    void initialize() override;

    void shutdown() override;

private:
    /** The subsystems used by this command. */

    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    Intake& intake = AbstractSubsystem::get_instance<Intake>();
};
