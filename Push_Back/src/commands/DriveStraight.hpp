#pragma once
#include "../Command.hpp"
#include "../subsystems/Drivetrain.hpp"
#include "../math/PID.hpp"
#include "../Config.hpp"

/**
 * A Command to make the robot drive in a straight line, facing its current heading, forwards or
 * backwards some number of inches.
 */
class DriveStraight : public ProgressCommand {
    /** The number of inches to drive forwards. */
    double target_distance;

    /** Amount of error to allow in the distance driven. */
    double tolerance;

    /** Maximum number of seconds this command is allowed to run. */
    double max_time;
    /** Timer to track the time this command has been running. */
    Timer timer;


public:
    /**
     * Constructor for the DriveStraight command.
     * @param inches The number of inches to drive. May be negative to drive backwards.
     * @param tolerance How close (in inches) the robot needs to be to the target distance to be
     * considered done.
     * @param max_time The maximum time (in seconds) this command is allowed to run. If the command
     * exceeds this time, it will be considered done. Default does not use this.
     */
    explicit DriveStraight(const double inches, const double tolerance,
        const double max_time = std::numeric_limits<double>::max())
        : target_distance(inches), tolerance(tolerance), max_time(max_time) {}

    void initialize() override;

    void periodic() override;

    void shutdown() override;

    bool is_complete() override;

    double get_progress() override;

private:
    /** Whether the command is done executing. */
    bool done = false;

    /**
     * The initial distance (inches) read from the drivetrain at the start of the command.
     * Needs to be updated in Drivetrain::initialize() to be accurate.
     */
    double initial_left_distance = 0;
    /**
     * The initial distance (inches) read from the drivetrain at the start of the command.
     * Needs to updated in Drivetrain::initialize() to be accurate.
     */
    double initial_right_distance = 0;

    /** The distance that has currently been covered for use in the progress command */
    double distance_driven = 0;


    /** The PID controller used to control the left drive motors. */
    PID pid_left = PID(Constants::PID::Drive::Distance::Kp,
                  Constants::PID::Drive::Distance::Ki,
                  Constants::PID::Drive::Distance::Kd);

    /** The PID controller used to control the right drive motors. */
    PID pid_right = PID(Constants::PID::Drive::Distance::Kp,
                  Constants::PID::Drive::Distance::Ki,
                  Constants::PID::Drive::Distance::Kd);

    /** The drivetrain subsystem. */
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
};
