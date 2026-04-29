#include "helpers/IdCommand.cc"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "architecture/commands/ParallelCommand.hpp"

class ParallelCommandImpl : public ParallelCommand {
public:
    ParallelCommandImpl(const std::initializer_list<std::unique_ptr<Command>> commands)
        : ParallelCommand(commands) {}

    void initialize() override {}
    void shutdown() override {}
};

void test5Cmds(ParallelCommandImpl& c, const std::vector<std::vector<std::string>>& logs) {
    // Initialize the parallel command
    c.run();
    ASSERT_FALSE(c.has_shutdown());
    ASSERT_EQ(c.calls, 1);

    // Initialize all the commands in parallel
    c.run();
    for (int i = 0; i < 5; ++i) {
        EXPECT_THAT(logs.at(i), testing::ElementsAre("initialize_"+ std::to_string(i)));
    }

    EXPECT_FALSE(c.has_shutdown());

    // Run all the commands in parallel
    c.run();
    for (int i = 0; i < 5; ++i) {
        // Check the log to make sure the inner command was run
        EXPECT_THAT(logs.at(i), testing::ElementsAre(
            "initialize_"+ std::to_string(i),
            "periodic_"+ std::to_string(i),
            "shutdown_"+ std::to_string(i)
        ));
    }

    // Shutdown the parallel command
    c.run();
    ASSERT_TRUE(c.has_shutdown());

    // Make sure not more changes
    for (int i = 0; i < 5; ++i) {
        EXPECT_THAT(logs.at(i), testing::ElementsAre(
            "initialize_"+ std::to_string(i),
            "periodic_"+ std::to_string(i),
            "shutdown_"+ std::to_string(i)
        ));
    }

    /* Expected state of the logs at the end of the test
    * logs[0] = <"initialize_0", "periodic_0", "shutdown_0">
    * logs[1] = <"initialize_1", "periodic_1", "shutdown_1">
    * logs[2] = <"initialize_2", "periodic_2", "shutdown_2">
    * logs[3] = <"initialize_3", "periodic_3", "shutdown_3">
    * logs[4] = <"initialize_4", "periodic_4", "shutdown_4"> */
}

/** Test fixture to make test setup a little neater. */
class ParallelCommandTest : public testing::Test {
protected:
    /** Parallel command to use for tests */
    ParallelCommandImpl parallel;

    /** Vector of commands to run. Idx=0 is the first command */
    std::vector<std::unique_ptr<Command>> commands_v;

    /** Logs for each of the 5 commands to keep track of which methods were run. */
    std::vector<std::vector<std::string>> logs;


    ParallelCommandTest() : parallel({}) {
        // Create 5 long vector
        logs.resize(5);

        for (int i = 0; i < 5; ++i) {
            commands_v.push_back(std::make_unique<IdCommand>(logs.at(i), i));
        }
    }
};

TEST_F(ParallelCommandTest, testSetCommands) {
    // Set commands using the `set_commands` method.
    parallel.set_commands(std::move(commands_v));

    test5Cmds(parallel, logs);
}

TEST_F(ParallelCommandTest, testAddCommand) {
    // Set commands by repeatedly calling `add_command`
    for (std::unique_ptr<Command>& cmd : commands_v) {
        parallel.add_command(std::move(cmd));
    }

    test5Cmds(parallel, logs);
}

TEST_F(ParallelCommandTest, testAddCommandAnd) {
    // Add commands with the `add_command_and` syntax
    parallel.add_command_and(std::move(commands_v.at(0)))
         .add_command_and(std::move(commands_v.at(1)))
         .add_command_and(std::move(commands_v.at(2)))
         .add_command_and(std::move(commands_v.at(3)))
         .add_command_and(std::move(commands_v.at(4)));

    test5Cmds(parallel, logs);
}

TEST_F(ParallelCommandTest, testInitializerList) {
    // Set commands using the initializer list constructor
    ParallelCommandImpl c({
        std::move(commands_v.at(0)),
        std::move(commands_v.at(1)),
        std::move(commands_v.at(2)),
        std::move(commands_v.at(3)),
        std::move(commands_v.at(4))
    });

    test5Cmds(c, logs);
}

TEST_F(ParallelCommandTest, testSingleCommand) {
    ParallelCommandImpl singleParallel = {std::move(commands_v.at(0))};
    singleParallel.run();  // Initialize the parallel

    singleParallel.run(); singleParallel.run();  // Initialize and shutdown inner command

    EXPECT_THAT(logs.at(0), testing::ElementsAre(
        "initialize_0",
        "periodic_0",
        "shutdown_0"
    ));

    singleParallel.run();
    EXPECT_TRUE(singleParallel.has_shutdown());

    EXPECT_THAT(logs.at(0), testing::ElementsAre(
        "initialize_0",
        "periodic_0",
        "shutdown_0"
    ));
}

TEST_F(ParallelCommandTest, testEmptyParallel) {
    parallel.run(); parallel.run();

    EXPECT_TRUE(parallel.has_shutdown());
}

TEST_F(ParallelCommandTest, testParallelCommandOneDifferent) {
    constexpr int lifespan = 4, id = 6;

    // Add the 5 commands from all prior tests
    parallel.set_commands(std::move(commands_v));

    std::vector<std::string> diff_log;
    std::unique_ptr<Command> diff_cmd = std::make_unique<IdCommand>(diff_log, id, lifespan);
    parallel.add_command(std::move(diff_cmd));

    parallel.run();  // Initialize the parallel command
    for (int i = 0; i < lifespan; ++i) {  // Run the inner commands
        parallel.run();
    }
    parallel.run();  // Shutdown the parallel command

    EXPECT_TRUE(parallel.has_shutdown());

    EXPECT_THAT(diff_log, testing::ElementsAre("initialize_6",
        "periodic_6",
        "periodic_6",
        "periodic_6",
        "shutdown_6"));

    EXPECT_THAT(logs.at(0), testing::ElementsAre(
        "initialize_0",
        "periodic_0",
        "shutdown_0"));

    /* Expected state of the logs at the end of the test
    * diff_log = <"initialize_6", "periodic_6", "periodic_6", "periodic_6", "shutdown_6">
    * logs[0] = <"initialize_0", "periodic_0", "shutdown_0">
    * logs[1] = <"initialize_1", "periodic_1", "shutdown_1">
    * logs[2] = <"initialize_2", "periodic_2", "shutdown_2">
    * logs[3] = <"initialize_3", "periodic_3", "shutdown_3">
    * logs[4] = <"initialize_4", "periodic_4", "shutdown_4"> */
}
