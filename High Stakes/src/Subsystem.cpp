#pragma once

#include "main.h"

/**
 * The framework used for all subsystems
 * Each subsystem runs every loop to control a specific thing
 * For example: the drivetrain subsystem controls movement by updating the power in each motor every loop
 */
class SubsystemAbstract {
public:
    /** Stores the subsystem's state */
    struct State {
        // Replace with the actual state variables of your subsystem
        // For instance:
        // bool isInitialized;
        // int errorCode;
    };
    
    virtual ~SubsystemAbstract() = default;

    /** Runs at when the robot first starts */
    virtual void initialize() = 0;

    /** Runs every loop (when robot is not disabled)  */
    virtual void periodic() = 0;

    /**
     * Runs every loop (when robot is disabled)
    */
    virtual void disabled_periodic() = 0;

    /** Runs when the robot shuts down */
    virtual void shutdown() = 0;

    /** Identification for logging */
    [[nodiscard]] virtual const char* id() const = 0;
};
