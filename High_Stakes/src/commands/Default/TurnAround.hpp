#pragma once
#include "../../Command.hpp"
#include "../../AbstractSubsystem.hpp"
#include "../../math/PID.hpp"

class Drivetrain;

/** This command flips the robot 180 degrees as fast and as accurate as possible */
class TurnAround : public Command {
    void initialize() override;
    void periodic() override;
    void shutdown() override;
    bool is_complete() override;

private:
    /** A reference to the drivetrain */
    // Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
    Drivetrain* drivetrain;
    /** The starting heading */
    double starting_heading;
    /** The target heading */
    double target_heading;
    PID heading_pid = PID(4200,0,0);
};