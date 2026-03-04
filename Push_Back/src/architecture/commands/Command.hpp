#pragma once

#include <queue>
#include "main.h"

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
    virtual ~Command() = default;

    /**
     * \brief High level method to control the execution of the command.
     * This method is intended to be called continuously.
     *
     * \details When called the first time, <code>initialize()</code> is called.
     * Then, every time after that, <code>periodic()</code> is called.
     * After each call to <code>periodic()</code>, <code>is_complete()</code> is checked.
     * On the call to run() where <code>is_complete()</code> returns true, <code>shutdown()</code>
     * is called within the same call to <code>run()</code>.
     * After <code>shutdown()</code> is called, the command is marked as completed and
     * <code>shutdown()</code> and <code>periodic</code> are not called again.
     *
     * \note If <code>is_complete()</code> returns true on the first call to <code>run()</code>,
     * then <code>initialize()</code> is called. On the second call to <code>run()</code>,
     * <code>periodic()</code> is called, <code>shutdown()</code> is called, and the command is
     * marked as completed.
     */
    void run();

    /** Return true when the command is complete. */
    virtual bool is_complete() = 0;

    /** Whether the command has completed and shutdown() has been called. */
    bool completed = false;

    /** The number of times a Command's <code>run()</code> method has been called. */
    unsigned int calls = 0;

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
};
