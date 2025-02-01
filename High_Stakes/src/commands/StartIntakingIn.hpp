#pragma once
#include "../Command.hpp"

class StartIntakingIn : public InstantCommand {
public:
    StartIntakingIn();
protected:
    void execute() override;
};
