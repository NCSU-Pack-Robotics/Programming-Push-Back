#pragma once

#include "../../include/main.h"

/**
 * A command is a thing the robot can do. They are intended to build off of each other,
 * creating a sequence of command that the robot can run. The robot is to only be controlled
 * through commands.
 *
 * In driver-control, each controller button is bound to a command. In autonomous, a sequence of
 * commands is run in a specific order to complete a task.
 */
class Command {
public:
    /** The number of times a Command's <code>run()</code> method has been called. */
    unsigned int calls = 0;

    virtual ~Command() = default;

    /**
     * Called when the command needs to be run. It will call initialize() once, then periodic()
     * until is_complete() returns true, then shutdown() once. If follow_up() returns a non-null
     * pointer, it will run that command next.
     */
    void run();

    /**
    * Use for checking if the command is done and can be discarded.
    * @return True if the command has been completed and <code>shutdown()</code> has been called.
    */
    [[nodiscard]] bool has_shutdown() const;

    /** Return true when the command is complete. */
    virtual bool is_complete() = 0;

    /** Called on the command's first loop right before periodic(). */
    virtual void initialize() = 0;

    /**
     * This method is very similar to a Subsystem's periodic method.
     * It is called every loop while the command is running.
     */
    virtual void periodic() = 0;

    /** Called once after the command has finished its job. */
    virtual void shutdown() = 0;

    /**
     * A command that will run right after this command is done.
     * If no follow up is needed, return nullptr.
     */
    // virtual std::unique_ptr<Command> follow_up();

    /**
     * @return some identifying string related to what the command does
     */
    [[nodiscard]] virtual std::string to_string() const;
    /**
     * @return the name of the command. Depending on which command type <code>to_string()</code> is called on,
     * <code>get_name()</code> will be used in different ways to generate the string
     */
    [[nodiscard]] virtual std::string get_name() const = 0;

private:
    /** Whether the command has completed and shutdown() has been called. */
    bool completed = false;
};
