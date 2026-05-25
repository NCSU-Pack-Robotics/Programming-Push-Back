#pragma once

#include <architecture/commands/InstantCommand.hpp>

class StopIntakeFull : public InstantCommand {
public:
    StopIntakeFull() = default;
protected:
    void execute() override;
};
