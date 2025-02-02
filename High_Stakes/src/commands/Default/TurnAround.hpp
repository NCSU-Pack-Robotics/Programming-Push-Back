#pragma once
#include "../../Command.hpp"
#include "../../subystems/Drivetrain.hpp"
#include "../../AbstractSubsystem.hpp"

/** This command flips the robot 180 degrees as fast and as accurate as possible */
class TurnAround : public Command {
    void initialize() override;
    void periodic() override;
    void shutdown() override;
    bool is_complete() override;

private:
    /** A reference to the drivetrain */
    // Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
    /** The starting pose. */
    double starting_heading;
};