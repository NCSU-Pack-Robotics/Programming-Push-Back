#pragma once

#include "Command.hpp"

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

protected:
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
