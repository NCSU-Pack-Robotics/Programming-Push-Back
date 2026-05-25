#pragma once

#include <architecture/commands/InstantCommand.hpp>

class StopIntakeEnd : public InstantCommand {
public:
    StopIntakeEnd() : InstantCommand("StopIntakeEnd") {};
protected:
    void execute() override;
};
