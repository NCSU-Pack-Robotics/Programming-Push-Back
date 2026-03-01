#pragma once

#include "Command.hpp"

/** A command that is only run once. Useful for setting things like motor speeds. */
class InstantCommand : public Command {
public:
    InstantCommand() = default;

    /** Constructor that allows for a function to be passed as the execute method */
    explicit InstantCommand(std::unique_ptr<std::function<void()>> executeFunction);

    /** Returns true because InstantCommands only run once */
    bool is_complete() override;

    /** Calls the execute function */
    void initialize() override;

    /** This is never called for an InstantCommand */
    void periodic() override;

    /** This is called but no code should be put in it.
     * Put everything inside execute. */
    void shutdown() override;

    /** The function that gets run. By default, it is set in the constructor,
     * but can be overwritten in child classes */
    virtual void execute();

private:
    /**
     * The execute function passed from the constructor
     * @return A unique pointer to a function that takes no arguments and returns nothing.
     */
    std::unique_ptr<std::function<void()>> executeFunction;
};

/** A command that can have its progress tracked and used via <code>get_progress()</code> */
class ProgressCommand : public Command {

public:
    /**
     * Tracks and returns the current progress of the ProgressCommand
     * @return A value between 0 and 1 representing the progress of the command. 0: no progress & 1: complete
     */
    virtual double get_progress() = 0;
};
