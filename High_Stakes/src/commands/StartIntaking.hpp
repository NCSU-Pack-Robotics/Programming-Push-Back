#pragma once
#include "../Command.hpp"

class StartIntaking : public InstantCommand {
public:
    StartIntaking();
protected:
    void execute() override;
};
