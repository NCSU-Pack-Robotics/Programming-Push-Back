#include <unordered_set>
#include <gtest/gtest.h>

#include "architecture/commands/Command.hpp"


class CommandImpl : public Command {
public:
    /** Vector of strings to keep track of which methods were run. */
    std::vector<std::string>& log;

    /** The number of calls to periodic() before the command is complete. */
    int lifespan;

    /**
     * @param log Vector of strings to keep track of which methods were run.
     * @param lifespan  The number of calls to periodic() before the command is complete.
     */
    explicit CommandImpl(auto& log, const int lifespan) : log(log), lifespan(lifespan) {
    }

    bool is_complete() override {
        return loops + 1 >= lifespan;
    }

    void initialize() override {
        this->log.emplace_back("initialize");
    }

    void periodic() override {
        this->log.push_back("periodic_" + std::to_string(loops));
    }

    void shutdown() override {
        this->log.emplace_back("shutdown");
    }
};


TEST(CommandTest, smokeTest) {
    std::vector<std::string> log;
    const std::unique_ptr<Command> cmd = std::make_unique<CommandImpl>(log, 5);

    // Initialize and run periodic 0
    cmd->run();

    // EXPECT first to get as much info as possible, then ASSERT to not go further
    EXPECT_EQ(log.front(), "initialize");
    EXPECT_EQ(log.back(), "periodic_0");
    ASSERT_EQ(log.size(), 2);  // Once for initialize and once for periodic 0

    // Run periodic 1-3
    for (int i = 1; i <= 3; ++i) {
        cmd->run();
        EXPECT_EQ(log.back(), "periodic_" + std::to_string(i));
        ASSERT_EQ(log.size(), 2 + i);
    }

    // Run periodic 4 and shutdown
    cmd->run();
    EXPECT_EQ(log.back(), "shutdown");
    EXPECT_EQ(log[log.size() - 2], "periodic_4");
    EXPECT_EQ(log.size(), 7);

    EXPECT_EQ(cmd->loops, 5);
}
