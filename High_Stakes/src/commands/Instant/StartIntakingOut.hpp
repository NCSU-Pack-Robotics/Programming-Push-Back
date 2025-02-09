#pragma once
#include "../../Command.hpp"
class StartIntakingOut : public InstantCommand {
public:
    StartIntakingOut();
protected:
    void execute() override;
};