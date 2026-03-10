#pragma once

#include "Command.hpp"

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

protected:
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
