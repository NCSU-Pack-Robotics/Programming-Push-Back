#include "ParallelCommand.hpp"

ParallelCommand::ParallelCommand(std::initializer_list<std::unique_ptr<Command>> commands) {
    // Initialize the vector
    this->commands = std::vector<std::unique_ptr<Command>>();

    // Move the commands from the initializer list to the vector
    for (auto& command : commands) {
        // cast away to `const`, and move the command to the vector
        this->commands.push_back(std::move(const_cast<std::unique_ptr<Command>&> (command)));
    }
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
    if (commands.size() == 1) {  // Edge case: one command
        commands[0]->run();

        if (commands[0]->has_shutdown()) {
            commands.clear();
        }

    } else {  // More than one command
        // Iterate through all commands
        for (auto it = commands.begin(); it != commands.end();) {
            // Run the command
            (*it)->run();

            // If the command is complete, remove its
            if ((*it)->has_shutdown()) {
                it = commands.erase(it);
            } else {
                ++it;
            }
        }
    }
}

bool ParallelCommand::is_complete() {
    return commands.empty();
}

std::string ParallelCommand::to_string() const {
    std::string result = get_name() + "([\n";
    for (size_t i = 0; i < commands.size(); i++) {
        std::string command_str = commands[i]->to_string();

        result += "  ";
        for (char c : command_str) {
            result += c;
            if (c == '\n') {
                result += "  ";
            }
        }

        if (i < commands.size() - 1) {
            result += ",";
        }
        result += "\n";
    }
    result += "])";
    return result;
}

std::string ParallelCommand::get_name() const
{
    return "ParallelCommand";
}
