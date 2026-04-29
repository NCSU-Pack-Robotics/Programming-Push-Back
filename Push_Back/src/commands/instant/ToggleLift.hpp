#pragma once

#include <architecture/commands/InstantCommand.hpp>

class ToggleLift : public InstantCommand
{
public:
    ToggleLift() = default;

    void execute() override;
};
