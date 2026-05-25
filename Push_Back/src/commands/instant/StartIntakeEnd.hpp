#pragma once

#include <architecture/commands/InstantCommand.hpp>

class StartIntakeEnd : public InstantCommand {
public:
    StartIntakeEnd() : InstantCommand("StartIntakeEnd") {};
protected:
    void execute() override;
};
