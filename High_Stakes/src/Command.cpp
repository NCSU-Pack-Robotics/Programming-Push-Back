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

void ChainCommand::periodic() {
    // If the queue is empty, return
    if (command_queue.empty())
        return;

    // Run the next command
    command_queue.front()->run();

    // If this command is complete, remove it
    if (command_queue.front()->is_complete()) {
        command_queue.pop();
    }
}

bool ChainCommand::is_complete() {
    return command_queue.empty();
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

void ParallelCommand::periodic() {
    // Iterate through all commands
    for (auto it = commands.begin(); it != commands.end();) {
        // Run the command
        (*it)->run();

        // If the command is complete, remove it
        if ((*it)->is_complete()) {
            it = commands.erase(it);
        } else {
            it++;
        }
    }
}

bool ParallelCommand::is_complete() {
    return commands.empty();
}

void InstantCommand::initialize() {
    this->execute();
}

void InstantCommand::execute() {
    executeFunction();
}

bool InstantCommand::is_complete() {
    return true;
}

void InstantCommand::periodic() {
    // This is never called
}

void InstantCommand::shutdown() {
    // This is called, but all logic for instant commands should be in execute
}
