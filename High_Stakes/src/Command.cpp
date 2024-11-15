#include "Command.hpp"

void Command::run() {
    // On first loop, run initialization
    if (loops == 0)
        initialize();

    // Run periodic
    if (!is_complete()) {
        periodic();
        loops++;
    } else {  // If complete, finish
        shutdown();
    }
}

void ChainCommand::add_command(std::unique_ptr<Command> command) {
    command_queue.push(std::move(command));
}

ChainCommand &ChainCommand::add_command_and(std::unique_ptr<Command> command) {
    command_queue.push(std::move(command));  // Call add_command

    return *this;  // Return a reference to this object
}

void ChainCommand::set_commands(std::queue<std::unique_ptr<Command>> commands) {
    command_queue = std::move(commands);
}

void ParallelCommand::add_command(std::unique_ptr<Command> command) {
    commands.push_back(std::move(command));
}

ParallelCommand &ParallelCommand::add_command_and(std::unique_ptr<Command> command) {
    commands.push_back(std::move(command));  // Call add_command

    return *this;  // Return a reference to this object
}

void ParallelCommand::set_commands(std::vector<std::unique_ptr<Command>> commands) {
    this->commands = std::move(commands);
}
