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
     * @param id
     * @param lifespan
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
