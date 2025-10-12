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
     * Called when the command needs to be run. It will call initialize() once, then periodic()
     * until is_complete() returns true, then shutdown() once. If follow_up() returns a non-null
     * pointer, it will run that command next.
     */
    void run();

    /** Return true when the command is complete. */
    virtual bool is_complete() = 0;

    /** The number of times a Command's <code>run()</code> method has been called. */
    unsigned int loops = 0;

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

/**
 * A class that runs several commands back-to-back.
 * This is useful for running multiple commands in a row.
 * Each command is run in the order they are added.
 */
class ChainCommand : public Command {
public:
    /**
     * A constructor that uses an initialize list to set the commands upon creation.
     * The first element in the list is the first command to run.
     * @param commands The commands to set.
     */
    ChainCommand(std::initializer_list<std::unique_ptr<Command>> commands);

    /**
     * Add a command to the end of the chain.
     * @param command The command to add.
     */
    void add_command(std::unique_ptr<Command> command);

    /**
     * Add a command to the chain and return a reference to the chain.
     * This is useful for adding multiple commands in a row.
     *
     * eg. chain.add_command_and(std::make_unique<Command1>())
     *          .add_command_and(std::make_unique<Command2>());
     *
     * @param command The command to add.
     * @return A reference to the chain.
     */
    ChainCommand& add_command_and(std::unique_ptr<Command> command);

    /**
     * Replaces the commands in the chain with list of commands.
     * @param commands The commands to set.
     */
    void set_commands(std::queue<std::unique_ptr<Command>> commands);

    /**
     * Runs the first command in the chain. When that command is done,
     * the next command in the chain is run. This continues until the last
     * command is run.
     */
    void periodic() override;

    /**
     * The function is complete when all commands in the chain have been completed and removed.
     */
    bool is_complete() override;

private:
    /** The queue of commands to run. */
    std::queue<std::unique_ptr<Command>> command_queue;
};

/**
 * A class that runs several commands at the same time.
 * This is useful for running multiple commands simultaneously.
 * When one command finishes, the rest of the commands continue to run until the last
 * command is finished.
 */
class ParallelCommand : public Command {
public:
    /**
     * A constructor that uses an initialize list to set the commands upon creation.
     * The order of commands in the list does not matter.
     * @param commands The commands to set.
     */
    ParallelCommand(std::initializer_list<std::unique_ptr<Command>> commands);

    /**
     * Add a command to the list of commands to run.
     * @param command The command to add.
     */
    void add_command(std::unique_ptr<Command> command);

    /**
     * Add a command to the list of commands to run and return a reference to the list.
     * This is useful for adding multiple commands in a row.
     *
     * eg. parallel.add_command_and(std::make_unique<Command1>())
     *              .add_command_and(std::make_unique<Command2>());
     *
     * @param command The command to add.
     * @return A reference to the list.
     */
    ParallelCommand& add_command_and(std::unique_ptr<Command> command);

    /**
     * Replaces the commands in the list with a new list of commands.
     * @param commands The commands to set.
     */
    void set_commands(std::vector<std::unique_ptr<Command>> commands);

    /**
     * Runs all commands in the list.
     * Because each command is being run in the same call to periodic(),
     * they will all run simultaneously.
     */
    void periodic() override;

    /**
     * The function is complete when all commands in the list have been completed.
     */
    bool is_complete() override;

private:
    /** The list of commands to run. */
    std::vector<std::unique_ptr<Command>> commands;
};

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