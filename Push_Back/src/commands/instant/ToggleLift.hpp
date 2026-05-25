#pragma once

#include <architecture/commands/InstantCommand.hpp>

class ToggleLift : public InstantCommand
{
public:
    ToggleLift() : InstantCommand("ToggleLift") {};

    void execute() override;
};
