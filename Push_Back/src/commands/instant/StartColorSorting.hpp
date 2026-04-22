#pragma once

#include <architecture/commands/InstantCommand.hpp>

class StartColorSorting : public InstantCommand {
public:
    StartColorSorting() = default;
protected:
    void execute() override;
};
