#pragma once
#include "../../Command.hpp"
#include "../../subsystems/LadyBrown.hpp"
#include "../../subsystems/Lift.hpp"

class LadyBrownLoadingLiftReverse : public Command {
    void initialize() override;
    void periodic() override;
    void shutdown() override;
    bool is_complete() override;

private:
    LadyBrown& lady_brown = AbstractSubsystem::get_instance<LadyBrown>();
    Lift& lift = AbstractSubsystem::get_instance<Lift>();

    bool complete = false;
    int32_t lift_start_degrees;
};