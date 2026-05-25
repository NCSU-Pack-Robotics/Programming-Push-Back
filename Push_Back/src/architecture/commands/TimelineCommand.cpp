#include "TimelineCommand.hpp"

#include <memory>

#include "ParallelCommand.hpp"
#include "ProgressCommand.hpp"

TimelineCommand::Checkpoint::Checkpoint(const double activationPoint,
                                        std::unique_ptr<Command> command)
    : activationPoint(activationPoint), command(std::move (command)) {}

TimelineCommand::TimelineCommand(std::unique_ptr<ProgressCommand> mainCommand,
                                 std::vector<Checkpoint>&& checkpoints,
                                 const bool wait_for_active_checkpoints)
    : ParallelCommand({}),
      mainCommand(std::move(mainCommand)),
      checkpoints(std::move(checkpoints)),
      wait_for_active_checkpoints(wait_for_active_checkpoints) {}

void TimelineCommand::periodic() {
    // This loop checks mainCommand progress to activate checkpoints
    if (this->mainCommand) {
        // progress called here to avoid calling a null pointer on the final loop
        const double progress = mainCommand->get_progress();

        // Run the main command
        if (this->mainCommand->has_shutdown()) {
            mainCommand = nullptr;
        } else {
            mainCommand->run();
        }

        // Add checkpoints if their activation points are reached
        for (auto it = checkpoints.begin(); it != checkpoints.end();) {
            if (it->activationPoint <= progress) {
                add_command(std::move(it->command));
                it = checkpoints.erase(it);  // Erase so it isn't added again
            } else {
                ++it;
            }
        }
    }

    // Runs all commands that were part of checkpoints. mainCommand does NOT get run here
    ParallelCommand::periodic();
}

bool TimelineCommand::is_complete() {
    if (wait_for_active_checkpoints && !ParallelCommand::is_complete())
        return false;

    if (this->mainCommand && !mainCommand->has_shutdown())
        return false;

    return true;
}

void TimelineCommand::initialize() {}

void TimelineCommand::shutdown() {}

