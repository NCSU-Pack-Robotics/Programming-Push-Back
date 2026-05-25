#pragma once

#include <architecture/commands/InstantCommand.hpp>

class StopColorSorting : public InstantCommand {
public:
    StopColorSorting() = default;
protected:
    void execute() override;
};
