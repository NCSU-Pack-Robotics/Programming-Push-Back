#pragma once
#include "../../Command.hpp"
class LadyBrownUp : public InstantCommand {
public:
    LadyBrownUp();
protected:
    void execute() override;
};