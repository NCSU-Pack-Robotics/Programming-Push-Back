#pragma once

#include <architecture/commands/InstantCommand.hpp>

class StopIntakeFull : public InstantCommand {
public:
    StopIntakeFull() : InstantCommand("StopIntakeFull") {};
protected:
    void execute() override;
};
