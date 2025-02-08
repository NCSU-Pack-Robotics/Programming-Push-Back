#pragma once
#include "../../Command.hpp"
#include "../../subystems/Lift.hpp"

class MoveLiftDegrees : public Command {
public:
    MoveLiftDegrees(int32_t degrees);
    void initialize() override;
    void periodic() override;
    void shutdown() override;
    bool is_complete() override;
private:
    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    int start_degree;
    int target_degree;
};