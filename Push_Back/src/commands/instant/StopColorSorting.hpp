#pragma once

#include <architecture/commands/InstantCommand.hpp>

class StopColorSorting : public InstantCommand {
public:
    StopColorSorting() : InstantCommand("StopColorSorting") {};
protected:
    void execute() override;
};
