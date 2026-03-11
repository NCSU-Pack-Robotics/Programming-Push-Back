#include "helpers/IdCommand.cc"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "architecture/commands/ChainCommand.hpp"

class ChainCommandImpl : public ChainCommand {
public:
    ChainCommandImpl(const std::initializer_list<std::unique_ptr<Command>> commands)
        : ChainCommand(commands) {}

    void initialize() override {}
    void shutdown() override {}
};

void test5Cmds(ChainCommandImpl& c, const std::vector<std::vector<std::string>>& logs) {
    // Initialize the chain command
    c.run();
    ASSERT_FALSE(c.has_shutdown());
    ASSERT_EQ(c.calls, 1);

    // Run each of the inner commands
    for (int i = 0; i < 5; ++i) {  // For each of the inner commands
        c.run(); c.run(); // Initialize and shutdown the inner command

        // Check the log to make sure the inner command was run
        EXPECT_THAT(logs.at(i), testing::ElementsAre(
            "initialize_"+ std::to_string(i),
            "periodic_"+ std::to_string(i),
            "shutdown_"+ std::to_string(i)
        ));
    }

    // Check logs again to make sure nothing was modified
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(logs.at(i).size(), 3);
    }

    // Shutdown the chain command
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
class ChainCommandTest : public testing::Test {
protected:
    /** Chain command to use for tests */
    ChainCommandImpl chain;

    /** Queue of commands to run */
    std::queue<std::unique_ptr<Command>> commands_q;

    /** Vector of commands to run. Idx=0 is the first command */
    std::vector<std::unique_ptr<Command>> commands_v;

    /** Logs for each of the 5 commands to keep track of which methods were run. */
    std::vector<std::vector<std::string>> logs;


    ChainCommandTest() : chain({}) {
        // Create 5 long vector
        logs.resize(5);

        for (int i = 0; i < 5; ++i) {
            commands_q.push(std::make_unique<IdCommand>(logs.at(i), i));
            commands_v.push_back(std::make_unique<IdCommand>(logs.at(i), i));
        }
    }
};

TEST_F(ChainCommandTest, testSetCommands) {
    // Set commands using the `set_commands` method.
    chain.set_commands(std::move(commands_q));

    test5Cmds(chain, logs);
}

TEST_F(ChainCommandTest, testAddCommand) {
    // Set commands by repeatedly calling `add_command`
    while (!commands_q.empty()) {
        chain.add_command(std::move(commands_q.front()));
        commands_q.pop();
    }

    test5Cmds(chain, logs);
}

TEST_F(ChainCommandTest, testAddCommandAnd) {
    // Add commands with the `add_command_and` syntax
    chain.add_command_and(std::move(commands_v.at(0)))
         .add_command_and(std::move(commands_v.at(1)))
         .add_command_and(std::move(commands_v.at(2)))
         .add_command_and(std::move(commands_v.at(3)))
         .add_command_and(std::move(commands_v.at(4)));

    test5Cmds(chain, logs);
}

TEST_F(ChainCommandTest, testInitializerList) {
    // Set commands using the initializer list constructor
    ChainCommandImpl c({
        std::move(commands_v.at(0)),
        std::move(commands_v.at(1)),
        std::move(commands_v.at(2)),
        std::move(commands_v.at(3)),
        std::move(commands_v.at(4))
    });

    test5Cmds(c, logs);
}

TEST_F(ChainCommandTest, testSingleCommand) {
    ChainCommandImpl singleChain = {std::move(commands_v.at(0))};
    singleChain.run();  // Initialize the chain

    singleChain.run(); singleChain.run();  // Initialize and shutdown inner command

    EXPECT_THAT(logs.at(0), testing::ElementsAre(
        "initialize_0",
        "periodic_0",
        "shutdown_0"
    ));

    singleChain.run();
    EXPECT_TRUE(singleChain.has_shutdown());

    EXPECT_THAT(logs.at(0), testing::ElementsAre(
        "initialize_0",
        "periodic_0",
        "shutdown_0"
    ));
}

TEST_F(ChainCommandTest, testEmptyChain) {
    chain.run(); chain.run();

    EXPECT_TRUE(chain.has_shutdown());
}
