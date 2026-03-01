#include "ChainCommand.hpp"

ChainCommand::ChainCommand(std::initializer_list<std::unique_ptr<Command>> commands) {
    // Initialize the queue
    command_queue = std::queue<std::unique_ptr<Command>>();

    // Move the commands from the initializer list to the queue
    for (auto& command : commands) {
        // cast away to `const`, and move the command to the queue
        command_queue.push(std::move(const_cast<std::unique_ptr<Command>&> (command)));
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
        command_queue.front()->run();  // Run the command one last time - will shutdown the command
        command_queue.pop();
    }
}

bool ChainCommand::is_complete() {
    return command_queue.empty();
}
