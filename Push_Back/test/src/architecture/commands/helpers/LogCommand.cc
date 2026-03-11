#pragma once

#include "architecture/commands/Command.hpp"

/** An implementation of <code>Command</code> to make testing possible. */
class LogCommand : public Command {
public:
    /** Vector of strings to keep track of which methods were run. */
    std::vector<std::string>& log;

    /**
     * @param log Vector of strings to keep track of which methods were run.
     * @param lifespan The number of calls to run() before the command is complete.
     */
    LogCommand(std::vector<std::string>& log, const int lifespan)
        : log(log), lifespan (lifespan) {}

    void initialize() override {
        this->log.emplace_back("initialize");
    }

    void periodic() override {
        this->log.push_back(std::format("periodic_{}", calls - 1));
    }

    void shutdown() override {
        this->log.emplace_back("shutdown");
    }

private:
    /** The number of calls to periodic() before the command is complete. */
    const int lifespan;

    bool is_complete() override {
        return calls >= lifespan;
    }
};
