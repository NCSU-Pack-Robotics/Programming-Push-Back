#pragma once

#include "../../subystems/Drivetrain.hpp"
#include "../../Timer.hpp"
#include "../../Command.hpp"

/** Very stupid driving method for going forwards some amount of time. */
class DumbDriveStraight : public Command {
    /** The time to drive for in seconds. */
    double time;
    /** The speed of the drive. */
    double power;

    /** The timer used to track the time. */
    Timer timer;

    /** A reference to the drivetrain subsystem. */
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

public:
    /**
     * Constructor for the DumbDriveStraight command.
     * @param power The power to drive at [-127, 127].
     * @param time The time to drive for in seconds.
     */
    DumbDriveStraight(double power, double time);

    void initialize() override;
    void periodic() override;
    void shutdown() override;
    bool is_complete() override;
};
