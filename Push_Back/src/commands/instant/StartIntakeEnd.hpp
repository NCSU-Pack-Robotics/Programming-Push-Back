#pragma once

#include <architecture/commands/InstantCommand.hpp>

class StartIntakeEnd : public InstantCommand {
public:
    StartIntakeEnd() = default;
protected:
    void execute() override;
};
