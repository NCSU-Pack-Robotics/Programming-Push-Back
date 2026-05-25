#include "ChainCommand.hpp"

ChainCommand::ChainCommand(std::initializer_list<std::unique_ptr<Command>> commands) {
    // Move the commands from the initializer list to the queue
    for (auto& command : commands) {
        // cast away to `const`, and move the command to the queue
        command_queue.push_back(std::move(const_cast<std::unique_ptr<Command>&> (command)));
    }
}

void ChainCommand::add_command(std::unique_ptr<Command> command) {
    command_queue.push_back(std::move(command));
}

ChainCommand &ChainCommand::add_command_and(std::unique_ptr<Command> command) {
    command_queue.push_back(std::move(command));  // Call add_command

    return *this;  // Return a reference to this object
}

void ChainCommand::set_commands(std::deque<std::unique_ptr<Command>> commands) {
    command_queue = std::move(commands);
}

void ChainCommand::periodic() {
    // If the queue is empty, return
    if (command_queue.empty())
        return;

    // Run the next command
    command_queue.front()->run();

    // If this command is complete, remove it
    if (command_queue.front()->has_shutdown()) {
        command_queue.pop_front();
    }
}

bool ChainCommand::is_complete() {
    return command_queue.empty();
}

std::string ChainCommand::to_string() const {
    std::string result = get_name() +  "(queue has " + std::to_string(command_queue.size()) + " commands left):\n";

    std::queue<std::unique_ptr<Command>> command_queue_temp;

    size_t index{};
    for (const auto& command : command_queue)
    {
        result += std::format("{}: {}\n", index++, command->to_string());
    }

    result += '\n';

    return result;
}

std::string ChainCommand::get_name() const
{
    return "ChainCommand";
}
