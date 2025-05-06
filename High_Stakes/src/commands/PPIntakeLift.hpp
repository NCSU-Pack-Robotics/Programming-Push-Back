#pragma once

#include "../asset.hpp"
#include "../Command.hpp"
#include "../subystems/Intake.hpp"
#include "../subystems/Lift.hpp"

/**
 * Used to follow path, intake a ring, and lift the ring onto a mobile goal simultaneously.
 */
class PPIntakeLift : public ParallelCommand {
public:
    /**
     * Constructor that sets the path to follow.
     * @param path The pah to follow.
     * @param lookahead The distance in inches for the lookahead distance
     * @param tolerance The number of inches from the end of the path to stop at.
     */
    PPIntakeLift(asset path,
        double lookahead = Constants::PathFollowing::LOOKAHEAD_DISTANCE,
        double tolerance = Constants::PathFollowing::STOP_DISTANCE);

    void initialize() override;

    void shutdown() override;

private:
    /** The subsystems used by this command. */

    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    Intake& intake = AbstractSubsystem::get_instance<Intake>();
};
