#pragma once
#include "Command.hpp"

class StartIntaking : public InstantCommand {
public:
    StartIntaking() : InstantCommand(){};
// TODO: Fix constructor errors
    // maybe pass execute into instant command??

protected:
    void execute() override;
};
