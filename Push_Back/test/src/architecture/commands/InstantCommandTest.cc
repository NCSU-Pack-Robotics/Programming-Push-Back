#include "architecture/commands/InstantCommand.hpp"

#include <gtest/gtest.h>

/** An implementation of <code>InstantCommand</code> to make testing possible. */
class ICommandImp : public InstantCommand {
    /** Vector of strings to keep track of what was run. */
    std::vector<std::string>& log;
public:
    /**
     * Construct an instant command for testing.
     * @param log Used to keep track of what was run.
     */
    explicit ICommandImp(std::vector<std::string>& log) : log(log) {}

    void execute() override {
        log.emplace_back("execute");
    }
};

/** Test the InstantCommand when subclassing overwrites the 'execute()' method. */
TEST(InstantCommandTest, testExecuteOverride) {
    std::vector<std::string> log;
    const std::unique_ptr<Command> cmd = std::make_unique<ICommandImp>(log);

    cmd->run();

    ASSERT_EQ(log.size(), 1);
    EXPECT_EQ(log.front(), "execute");
    EXPECT_FALSE(cmd->has_shutdown());
    EXPECT_EQ(cmd->calls, 1);

    /* Expected state of the log at the end of the test
     * <"execute"> */
}

/** Test the InstantCommand when the function to execute is passed from the constructor. */
TEST(InstantCommandTest, testConstructor) {
    std::vector<std::string> log;
    auto log_ptr = &log;

    // Construct an Instant command that makes a log entry
    const std::unique_ptr<Command> cmd = std::make_unique<InstantCommand>(
    [log_ptr] { log_ptr->emplace_back("execute"); });

    cmd->run();

    ASSERT_EQ(log.size(), 1);
    EXPECT_EQ(log.front(), "execute");
    EXPECT_FALSE(cmd->has_shutdown());
    EXPECT_EQ(cmd->calls, 1);

    /* Expected state of the log at the end of the test
     * <"execute"> */
}
