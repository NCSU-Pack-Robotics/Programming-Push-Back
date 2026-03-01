#include "InstantCommand.hpp"

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

InstantCommand::InstantCommand(std::unique_ptr<std::function<void()>> executeFunction) {
    this->executeFunction = std::move(executeFunction);
}
