#pragma once

/**
 * The framework used for all subsystems
 * Each subsystem runs every loop to control a specific thing
 * For example: the drivetrain subsystem controls movement by updating the power in each motor every loop
 */
class AbstractSubsystem {
public:
    // Delete copy constructor and assignment operator
    AbstractSubsystem(const AbstractSubsystem &) = delete;
    AbstractSubsystem &operator=(const AbstractSubsystem &) = delete;

    // Delete move constructor and assignment operator
    AbstractSubsystem(AbstractSubsystem &&) = delete;
    AbstractSubsystem &operator=(AbstractSubsystem &&) = delete;

    /**
     * Get the instance of the subsystem.
     * @return The instance of the subsystem
     */
    template<typename T>
    static T &get_instance() {
        // A static instance of the subsystem
        static T instance;

        // Return the instance
        return instance;
    }

    /** Runs at when the robot first starts. */
    virtual void initialize() = 0;

    /** Runs every loop (when robot is not disabled)  */
    virtual void periodic() = 0;

    /** Runs every loop (when robot is disabled) */
    virtual void disabled_periodic() = 0;

    /** Runs when the robot shuts down */
    virtual void shutdown() = 0;

protected:
    /**
     * Constructor for AbstractSubsystem.
     * Takes no arguments and is protected to ensure only subclasses can instantiate.
     */
    AbstractSubsystem() = default;
};
