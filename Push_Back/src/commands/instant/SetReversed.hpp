#pragma once
#include "architecture/commands/InstantCommand.hpp"

class SetReversed final : public InstantCommand {
public:
    explicit SetReversed(const bool reversed) : reversed(reversed) {};
protected:
    void execute() override;

    std::string to_string() const override;

private:
    bool reversed;
};
