#include "LogCommand.cc"

/**
 * Used for testing commands that store other commands.
 * This is a Log command so we can see which methods were executed.
 */
class IdCommand : public LogCommand {
    /** Used to identify a command. */
    const int id;

public:
    /**
     * Constructs an ID command and its superclass.
     * @param log The log for
     * @param id Some number to represent the command. Should be unique in the context of the
     * test to avoid mixing up which command is which in the log.
     * @param lifespan The number of calls to run() before the command is complete.
     */
    IdCommand(std::vector<std::string>& log, const int id, const int lifespan = 0)
        : LogCommand(log, lifespan), id(id) {}

    void initialize() override {
        log.emplace_back(std::format("initialize_{}", id));
    }

    void periodic() override {
        log.emplace_back(std::format("periodic_{}", id));
    }

    void shutdown() override {
        log.emplace_back(std::format("shutdown_{}", id));
    }
};
