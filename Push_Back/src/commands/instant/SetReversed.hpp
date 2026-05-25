#pragma once
#include "architecture/commands/InstantCommand.hpp"

class SetReversed final : public InstantCommand {
public:
    explicit SetReversed(const bool reversed) :
        InstantCommand("SetReversed(" + std::to_string(reversed) + ")"), reversed(reversed) {};

    void execute() override;

private:
    bool reversed;
};
