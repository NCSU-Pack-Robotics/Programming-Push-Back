#include "InstantCommand.hpp"

void InstantCommand::initialize() {
    this->execute();
}

void InstantCommand::execute() {
    executeFunction();
}

void InstantCommand::shutdown() {
    // Called once, right after is_complete() returns true.
}

bool InstantCommand::is_complete() {
    return true;
}

void InstantCommand::periodic() {
    // This is called once, right after initialize()
}

InstantCommand::InstantCommand(std::function<void()> executeFunction, std::string description) {
    this->executeFunction = std::move(executeFunction);
    this->description = std::move(description);
}

std::string InstantCommand::to_string() const {
    return "InstantCommand(" + description + ")";
}