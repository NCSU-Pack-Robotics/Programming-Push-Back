#pragma once

#include <architecture/commands/InstantCommand.hpp>

class StartColorSorting : public InstantCommand {
public:
    StartColorSorting() : InstantCommand("StartColorSorting") {};
protected:
    void execute() override;
};
