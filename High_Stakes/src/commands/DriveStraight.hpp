#pragma once
#include "../Command.hpp"
#include "../subystems/drivetrain.hpp"
#include "../math/PID.hpp"
#include "../Config.hpp"

/**
 * A Command to make the robot drive in a straight line, facing its current heading, forwards or
 * backwards some number of inches.
 */
class DriveStraight : public Command {
    /** The number of inches to drive forwards. */
    double target_distance;

    /** Amount of error to allow in the distance driven. */
    double tolerance;

public:
    /**
     * Constructor for the DriveStraight command.
     * @param inches The number of inches to drive. May be negative to drive backwards.
     * @param tolerance How close the robot needs to be to the target distance to be
     * considered done.
     */
    explicit DriveStraight(const double inches, const double tolerance)
        : target_distance(inches), tolerance(tolerance) {}

    void initialize() override;

    void periodic() override;

    void shutdown() override;

    bool is_complete() override;

private:
    /** Whether the command is done executing. */
    bool done = false;

    /** The initial distance read from the drivetrain at the start of the command. */
    double initial_left_distance = 0;
    /** The initial distance read from the drivetrain at the start of the command. */
    double initial_right_distance = 0;


    /** The PID controller used to control the drive motors. */
    PID pid = PID(Constants::PID::Drive::Distance::Kp,
                  Constants::PID::Drive::Distance::Ki,
                  Constants::PID::Drive::Distance::Kd);

    /** The drivetrain subsystem. */
    Drivetrain &drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
};
