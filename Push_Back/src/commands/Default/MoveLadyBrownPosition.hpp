#pragma once
#include "../../AbstractSubsystem.hpp"
#include "../../Command.hpp"
#include "../../subsystems/LadyBrown.hpp"

class MoveLadyBrownPosition : public Command {
public:
    MoveLadyBrownPosition(LadyBrown::Position position);
    void initialize() override;
    void periodic() override;
    void shutdown() override;
    bool is_complete() override;

private:
    LadyBrown& lady_brown = AbstractSubsystem::get_instance<LadyBrown>();

    LadyBrown::Position position;
};