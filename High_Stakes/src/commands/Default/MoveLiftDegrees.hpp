#pragma once
#include "../../Command.hpp"
#include "../../subystems/Lift.hpp"

class MoveLiftDegrees : public Command {
public:
    /** A command that moves the lift a certain amount of degrees. */
    MoveLiftDegrees(int32_t degrees);
    void initialize() override;
    void periodic() override;
    void shutdown() override;
    bool is_complete() override;
private:
    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    /** The degree the lift starts at */
    int start_degree;
    /** The start degree + the amount of degrees to turn */
    int target_degree;
};