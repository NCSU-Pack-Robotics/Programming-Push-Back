#pragma once

#include <memory>
#include <vector>

#include "ParallelCommand.hpp"
#include "ProgressCommand.hpp"

/**
 * A command that follows the progression of a <code>ProgressCommand</code> and activates additional
 * commands at certain Checkpoints that are done in parallel to the main command.
 */
class TimelineCommand : public ParallelCommand {
public:
    /**
    * A Checkpoint contains both an activation point along the main command
    * and a command to be executed at that point.
    */
    struct Checkpoint {
        /**
        * The actual constructor for Checkpoint. Takes in an activation point corresponding to
        * an amount of progress along the main command, and a command that starts at that point.
        * @param activationPoint The amount of progress along the main command at which another
        * command activates. Should be between 0 and 1.
        * @param command The command to be executed at the activation point.
        */
        Checkpoint(double activationPoint, std::unique_ptr<Command> command);

        /** The amount of progress along the main command at which another command activates.
         * Should be between 0 and 1. */
        double activationPoint;

        /** The command to be executed at the activation point. */
        std::unique_ptr<Command> command;
    };

    /**
    * A constructor that uses a main ProgressCommand and an initializer list of checkpoints to
    * activate additional commands.
    * @param mainCommand The command whose progress is tracked to activate other commands.
    * @param checkpoints The list of Checkpoints that will activate throughout mainCommand.
    * @param wait_for_active_checkpoints If true, the TimelineCommand will not finish until all
    * checkpoints that have started shutdown. If False, checkpoints are not guaranteed to finish,
    * even if they initialize.
    */
    TimelineCommand(std::unique_ptr<ProgressCommand> mainCommand,
                    std::vector<Checkpoint>&& checkpoints,
                    bool wait_for_active_checkpoints = false);

    /**
    * Runs mainCommand and any other commands from Checkpoints, and checks mainCommand progress
    * to activate more Checkpoints.
    */
    void periodic() override;

    bool is_complete() override;
    void initialize() override;

private:

    /** The main command whose progress is tracked to activate other commands. */
    std::unique_ptr<ProgressCommand> mainCommand;
    /** The list of Checkpoints that will activate throughout mainCommand. */
    std::vector<Checkpoint> checkpoints;
    /** Whether the TimelineCommand should wait for checkpoints to finish before finishing itself. */
    const bool wait_for_active_checkpoints;
};
