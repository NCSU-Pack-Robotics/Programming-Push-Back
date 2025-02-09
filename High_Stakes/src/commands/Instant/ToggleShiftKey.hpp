#pragma once
#include "../../Command.hpp"

class ToggleShiftKey : public InstantCommand {
public:
    ToggleShiftKey();
protected:
    void execute() override;
};