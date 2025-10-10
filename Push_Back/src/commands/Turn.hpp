#pragma once

#include "../Command.hpp"
#include "../Config.hpp"
#include "../subsystems/Drivetrain.hpp"
#include "../math/PID.hpp"

/**
 * A Command to make the robot turn in (CCW/CW) place some number of degrees.
 */
class Turn : public Command {
    /** The number of inches to drive forwards. */
    double target_degrees;

    /** Amount of error to allow in the distance driven. */
    double tolerance;

public:
    /**
     * Constructor for the Turn command.
     * @param degrees The number of degrees to turn. Positive will turn CCW, negative will turn CW.
     * @param tolerance How many degrees of error may be allowed before the command is done.
     */
    Turn(const double degrees, const double tolerance)
     : target_degrees(degrees), tolerance(tolerance) {};

    void initialize() override;

    void periodic() override;

    void shutdown() override;

    bool is_complete() override;

    std::string to_string() const override;

private:
    /** Whether the command is done executing. */
    bool done = false;

    /** The heading of the robot when the command was initialized. */
    double initial_heading = 0;  // Needs to be updated on initialization

    /** The PID controller used to control the drive motors. */
    PID pid = PID(Constants::PID::Drive::Degrees::Kp,
                  Constants::PID::Drive::Degrees::Ki,
                  Constants::PID::Drive::Degrees::Kd);

    /** The drivetrain subsystem. */
    Drivetrain &drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
};
