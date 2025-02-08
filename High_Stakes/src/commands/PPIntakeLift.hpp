#pragma once
#include "AbstractSubsystem.hpp"
#include "asset.hpp"
#include "Command.hpp"
#include "Intake.hpp"
#include "Lift.hpp"

/**
 * Used to follow path, intake a ring, and lift the ring onto a mobile goal simultaneously.
 */
class PPIntakeLift : public ParallelCommand {
public:
    /**
     * Constructor that sets the path to follow.
     * @param path The pah to follow.
     */
    PPIntakeLift(asset path);

    void initialize() override;

    void shutdown() override;

private:
    /** The subsystems used by this command. */

    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    Intake& intake = AbstractSubsystem::get_instance<Intake>();
};
