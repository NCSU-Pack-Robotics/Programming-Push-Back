#pragma once

#include "Command.hpp"

#include <utility>

/** A command that is only run once. Useful for setting things like motor speeds. */
class InstantCommand : public Command {
protected:
    InstantCommand(std::string_view description = "") : description(description) {};
public:

    /** Constructor that allows for a function to be passed as the execute method */
    explicit InstantCommand(std::function<void()> executeFunction, std::string_view description = "") :
        executeFunction(std::move(executeFunction)), description(description) {};

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

    /**
     * @return The string: InstantCommand(description)
     */
    [[nodiscard]] std::string to_string() const override;

    /** @return The description passed when creating the command */
    [[nodiscard]] std::string get_name() const override;

private:
    /**
     * The execute function passed from the constructor
     * @return A function that takes no arguments and returns nothing.
     */
    std::function<void()> executeFunction;

    /** Optional description */
    std::string description;
};
