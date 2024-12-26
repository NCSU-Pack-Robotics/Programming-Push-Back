#pragma once
#include "../Command.hpp"

class StopIntaking : public InstantCommand {
public:
    StopIntaking();
protected:
    void execute() override;
};
