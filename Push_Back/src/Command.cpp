#include "Command.hpp"

void Command::run() {
    // On first loop, run initialization
    if (loops == 0)
        initialize();

    // Run periodic
    if (!is_complete()) {
        periodic();
    } else {  // If complete, finish
        shutdown();
    }

    /* This values must be updated, unconditionally, at the end of the run() method. Previously,
     * we placed right after the call to periodic() in the if statement. This caused Instant
     * Command to run twice b/c they're always complete and never run periodic() */
    loops++;
}

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

std::string ChainCommand::to_string() const {
    std::string result = "ChainCommand(queue: " + std::to_string(command_queue.size()) + " commands left";
    
    if (!command_queue.empty()) {
        result += ",\n  current: ";
        std::string command_str = command_queue.front()->to_string();
        
        for (char c : command_str) {
            result += c;
            if (c == '\n') {
                result += "  ";
            }
        }
    }
    
    result += ")";
    return result;
}

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

        if (commands[0]->is_complete()) {
            commands[0]->shutdown();
            commands.clear();
        }

    } else {  // More than one command
        // Iterate through all commands
        for (auto it = commands.begin(); it != commands.end();) {
            // Run the command
            (*it)->run();

            // If the command is complete, remove its
            if ((*it)->is_complete()) {
                (*it)->run();  // Run the command one last time - will shutdown the command
                it = commands.erase(it);
            } else {
                ++it;
            }
        }
    }
}

std::string ParallelCommand::to_string() const {
    std::string result = "ParallelCommand([\n";
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

bool ParallelCommand::is_complete() {
    return commands.empty();
}

void InstantCommand::initialize() {
    this->execute();
}

void InstantCommand::execute() {
    (*executeFunction)();
}

void InstantCommand::shutdown() {
}

bool InstantCommand::is_complete() {
    return true;
}

void InstantCommand::periodic() {
    // This is never called
}

InstantCommand::InstantCommand(std::unique_ptr<std::function<void()>> executeFunction, std::string description) {
    this->executeFunction = std::move(executeFunction);
    this->description = std::move(description);
}

std::string InstantCommand::to_string() const {
    return "InstantCommand(" + description + ")";
}
