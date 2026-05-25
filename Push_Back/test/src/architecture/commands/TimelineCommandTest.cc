#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "architecture/commands/TimelineCommand.hpp"
#include "architecture/commands/ProgressCommand.hpp"

#include "helpers/IdCommand.cc"

/** Completes when it is called 10 times. Progress is number of calls out of 10. */
class Count2Ten : public ProgressCommand {
public:
    double get_progress() override {
        return calls / 10.0;
    }

    void initialize() override {}

    void periodic() override {}

    void shutdown() override {}

protected:
    bool is_complete() override {
        return calls >= 10;
    }
};

/** Test fixture to consolidate some repeated code. */
class TimelineCommandTest : public testing::Test {
protected:
    /** Log to keep track of which methods were called on which commands. */
    std::vector<std::string> log;
    /** Checkpoints to be used in the tests. */
    std::vector<TimelineCommand::Checkpoint> checkpoints;
};

TEST_F(TimelineCommandTest, test0Checkpoint) {
    checkpoints.emplace_back(0, std::make_unique<IdCommand>(log, 1));
    TimelineCommand tc(std::make_unique<Count2Ten>(), std::move(checkpoints));

    tc.run(); // Initialize the tc
    tc.run(); // Runs the periodic of the tc once
    ASSERT_THAT(log, testing::ElementsAre("initialize_1"));
    tc.run(); // Should run periodic and shutdown of the IdCommand with lifetime 0
    ASSERT_THAT(log, testing::ElementsAre("initialize_1", "periodic_1", "shutdown_1"));

    // Should not run the command again
    while (!tc.has_shutdown()) tc.run();
    ASSERT_THAT(log, testing::ElementsAre("initialize_1", "periodic_1", "shutdown_1"));
}

TEST_F(TimelineCommandTest, testMultipleCheckpoints) {
    checkpoints.emplace_back(0.2, std::make_unique<IdCommand>(log, 1));
    checkpoints.emplace_back(0.3, std::make_unique<IdCommand>(log, 2));
    checkpoints.emplace_back(0.7, std::make_unique<IdCommand>(log, 3));
    TimelineCommand tc(std::make_unique<Count2Ten>(), std::move(checkpoints));

    tc.run(); // Initialize the tc
    tc.run(); // Runs the periodic of the tc once
    ASSERT_THAT(log, testing::ElementsAre());
    tc.run(); // Should run initialize of the main command and nothing else
    ASSERT_THAT(log, testing::ElementsAre());
    tc.run(); // Should run periodic of the main command and initialize and periodic of IdCommand 1
    ASSERT_THAT(log, testing::ElementsAre("initialize_1"));
    tc.run(); // Should run periodic of the main command and shutdown of IdCommand 1 and initialize and periodic of IdCommand 2
    ASSERT_THAT(log, testing::ElementsAre("initialize_1", "periodic_1", "shutdown_1", "initialize_2"));
    tc.run(); // Finish off command 2
    ASSERT_THAT(log, testing::ElementsAre("initialize_1", "periodic_1", "shutdown_1", "initialize_2", "periodic_2", "shutdown_2"));
    log.clear(); // reset the log
    while (!tc.has_shutdown()) tc.run(); // Finish off the main command and command 3
    ASSERT_THAT(log, testing::ElementsAre("initialize_3", "periodic_3", "shutdown_3"));
}

TEST_F(TimelineCommandTest, testOverlappingCheckpoints) {
    checkpoints.emplace_back(0.2, std::make_unique<IdCommand>(log, 1, 2));
    checkpoints.emplace_back(0.3, std::make_unique<IdCommand>(log, 2, 2));
    TimelineCommand tc(std::make_unique<Count2Ten>(), std::move(checkpoints));

    tc.run(); // Initialize the tc
    tc.run(); // Runs the periodic of the tc once
    ASSERT_THAT(log, testing::ElementsAre());
    tc.run(); // Should run initialize of the main command and nothing else
    ASSERT_THAT(log, testing::ElementsAre());
    tc.run(); // Should run periodic of the main command and initialize and periodic of IdCommand 1
    ASSERT_THAT(log, testing::ElementsAre("initialize_1"));
    tc.run(); // Should run periodic of the main command and periodic of IdCommand 1 and initialize and periodic of IdCommand 2
    ASSERT_THAT(log, testing::ElementsAre("initialize_1", "periodic_1", "shutdown_1", "initialize_2"));
    tc.run(); // Should run periodic of the main command and shutdown of IdCommand 1 and periodic of IdCommand 2
    ASSERT_THAT(log, testing::ElementsAre("initialize_1", "periodic_1",  "shutdown_1", "initialize_2", "periodic_2", "shutdown_2"));
    log.clear();
    while (!tc.has_shutdown()) tc.run(); // Finish off the main command
    ASSERT_THAT(log, testing::ElementsAre());
}

TEST_F(TimelineCommandTest, testCheckpointAtEnd)  {
    // Will not run because the activation point is at the very end of the main command.
    checkpoints.emplace_back(1, std::make_unique<IdCommand>(log, 2));

    checkpoints.emplace_back(0.9, std::make_unique<IdCommand>(log, 1));
    TimelineCommand tc(std::make_unique<Count2Ten>(), std::move(checkpoints));

    tc.run(); // Initialize the tc
    tc.run(); // Runs the periodic of the tc once
    ASSERT_THAT(log, testing::ElementsAre());
    while (!tc.has_shutdown()) tc.run(); // Finish off the main command and run the checkpoint command

    /* Only initialize is run because the main command is already done by the time this
     * checkpoint is set to activate. In the future this may cause problems, in which case, some
     * things will have to be reworked. */
    ASSERT_THAT(log, testing::ElementsAre("initialize_1"));
}

TEST_F(TimelineCommandTest, testWaitForCheckpoints) {
    checkpoints.emplace_back(0, std::make_unique<IdCommand>(log, 1, 0));
    checkpoints.emplace_back(0.9, std::make_unique<IdCommand>(log, 2, 20));
    TimelineCommand tc(std::make_unique<Count2Ten>(), std::move(checkpoints), true);

    tc.run(); // Initialize the tc
    tc.run(); // Runs the periodic of the tc once
    ASSERT_THAT(log, testing::ElementsAre("initialize_1"));
    tc.run(); // Should run periodic of the main command and periodic of IdCommand 1
    ASSERT_THAT(log, testing::ElementsAre("initialize_1", "periodic_1", "shutdown_1"));
    log.clear();

    while (!tc.has_shutdown()) tc.run(); // Finish off the main command and run the checkpoint command
    EXPECT_THAT(log, testing::Contains("initialize_2"));
    EXPECT_THAT(log, testing::Contains("periodic_2").Times(19));
    EXPECT_THAT(log, testing::Contains("shutdown_2"));
}
