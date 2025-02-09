#pragma once
#include "../../Command.hpp"
class ToggleClamp : public InstantCommand {
public:
    ToggleClamp();
protected:
    void execute() override;
};