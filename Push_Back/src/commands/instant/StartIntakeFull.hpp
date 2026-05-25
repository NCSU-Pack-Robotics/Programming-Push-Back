#pragma once

#include <architecture/commands/InstantCommand.hpp>

class StartIntakeFull : public InstantCommand {
public:
    StartIntakeFull() : InstantCommand("StartIntakeFull") {};
protected:
    void execute() override;
};
