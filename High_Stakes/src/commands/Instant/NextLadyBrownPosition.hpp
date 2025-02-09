#pragma once
#include "../../Command.hpp"
class NextLadyBrownPosition : public InstantCommand {
public:
    NextLadyBrownPosition();
protected:
    void execute() override;
};