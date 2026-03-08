#include <unordered_set>
#include <gtest/gtest.h>

#include "architecture/commands/Command.hpp"
#include "helpers/LogCommand.cc"

/**
 * Smoke test: Most popular use case: a command that runs for a few loops before completing.
 */
TEST(CommandTest, smokeTest) {
    std::vector<std::string> log;
    const std::unique_ptr<Command> cmd = std::make_unique<LogCommand>(log, 6);

    // Initialize and run periodic 0
    cmd->run();

    // EXPECT first to get as much info as possible, then ASSERT to not go further
    EXPECT_EQ(log.front(), "initialize");
    EXPECT_EQ(cmd->calls, 1);
    EXPECT_FALSE(cmd->has_shutdown());
    ASSERT_EQ(log.size(), 1);  // Once for initialize and once for periodic 0

    // Run periodic [2, 5]
    for (int call = 2; call <= 5; ++call) {
        cmd->run();

        // First call is to initialize, so (call-1) is the periodic call number
        const int periodic_call = call - 1;

        EXPECT_EQ(log.back(), "periodic_"+ std::to_string(periodic_call));
        ASSERT_EQ(log.size(), call);
    }
    EXPECT_FALSE(cmd->has_shutdown());

    // Run periodic 4 and shutdown
    cmd->run();
    EXPECT_EQ(log.back(), "shutdown");
    EXPECT_EQ(log.at(log.size() - 2), "periodic_5");
    EXPECT_TRUE(cmd->has_shutdown());
    EXPECT_EQ(log.size(), 7);

    EXPECT_EQ(cmd->calls, 6);

    /* Expected state of the log at the end of the test
     * <i=0, ..., i=6>
     * <"initialize", "periodic_1", "periodic_2", "periodic_3", "periodic_4", "periodic_5", "shutdown"> */
}

/**
 * Edge case: a command that is complete on the first run.
 * Instant commands fall into this case.
 */
TEST(CommandTest, completeOnFirstRun) {
    std::vector<std::string> log;
    const std::unique_ptr<Command> cmd = std::make_unique<LogCommand>(log, 0);

    cmd->run();
    ASSERT_EQ(log.size(), 1);
    EXPECT_EQ(log.front(), "initialize");
    EXPECT_FALSE(cmd->has_shutdown());
    ASSERT_EQ(cmd->calls, 1);

    cmd->run();
    ASSERT_EQ(log.size(), 3);
    EXPECT_EQ(log.front(), "initialize");
    EXPECT_EQ(log.at(1), "periodic_1");
    EXPECT_EQ(log.back(), "shutdown");
    EXPECT_TRUE(cmd->has_shutdown());
    EXPECT_EQ(cmd->calls, 2);

    /* Expected state of the log at the end of the test
     * <i=0, ..., i=2>
     * <"initialize", "periodic_1", "shutdown"> */
}
