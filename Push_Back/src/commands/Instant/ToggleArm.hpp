#pragma once
#include "../../Command.hpp"
class ToggleArm : public InstantCommand {
public:
    ToggleArm();
protected:
    void execute() override;
};