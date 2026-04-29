#pragma once

#include <architecture/commands/InstantCommand.hpp>

class StartIntakeFull : public InstantCommand {
public:
    StartIntakeFull() = default;
protected:
    void execute() override;
};
