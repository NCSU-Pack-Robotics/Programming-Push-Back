#include "InstantCommand.hpp"

void InstantCommand::initialize() {
    this->execute();
}

void InstantCommand::execute() {
    (*executeFunction)();
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

InstantCommand::InstantCommand(std::unique_ptr<std::function<void()>> executeFunction) {
    this->executeFunction = std::move(executeFunction);
}
