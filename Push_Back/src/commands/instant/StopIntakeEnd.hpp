#pragma once

#include <architecture/commands/InstantCommand.hpp>

class StopIntakeEnd : public InstantCommand {
public:
    StopIntakeEnd() = default;
protected:
    void execute() override;
};
