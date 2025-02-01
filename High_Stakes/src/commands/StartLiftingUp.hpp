#pragma once
#include "../Command.hpp"

class StartLiftingUp : public InstantCommand {
public:
    StartLiftingUp();
protected:
    void execute() override;
};