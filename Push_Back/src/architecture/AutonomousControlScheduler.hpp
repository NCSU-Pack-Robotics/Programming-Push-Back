#pragma once
#include "architecture/AbstractSubsystem.hpp"
#include "Command.hpp"
#include "subsystems/Drivetrain.hpp"

/**
 * A subclass of ChainCommand. It is used during the autonomous portion of the event.
 * This is a chain command because autonomous consists of a series of actions back-to-back.
 */
class AutonomousControlScheduler final : public ChainCommand {
public:
    /**
     * Constructs an instance of AutonomousControlScheduler.
     * The chain of commands is created in this constructor.
     */
    AutonomousControlScheduler();

    void initialize() override;

    void shutdown() override;

private:
    /** A reference to the Drivetrain */
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
};
