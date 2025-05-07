#include "AutonomousControlScheduler.hpp"

#include "asset.hpp"
#include "commands/Default/MoveLadyBrownPosition.hpp"
#include "commands/Default/TurnAround.hpp"
#include "commands/Default/TurnToHeading.hpp"
#include "commands/DriveStraight.hpp"
#include "commands/Instant/SetPose.hpp"
#include "commands/Instant/SetReversed.hpp"
#include "commands/Instant/ToggleArm.hpp"
#include "commands/Instant/ToggleClamp.hpp"
#include "commands/PPIntakeLift.hpp"
#include "commands/PurePursuit.hpp"
#include "commands/default/DriveStraightIntakeLift.hpp"
#include "subystems/Clamp.hpp"
#include "subystems/LadyBrown.hpp"
#include "subystems/Lift.hpp"

// Global paths defined here
ASSET(rushCenterFromLeft_txt)
ASSET(alignForGoal_txt)
ASSET(getGoal_txt)
ASSET(intake1_txt)
ASSET(intakeCorner_txt)

using namespace std;

AutonomousControlScheduler::AutonomousControlScheduler(): ChainCommand({}) {
    Clamp& clamp = AbstractSubsystem::get_instance<Clamp>();
    Lift& lift = AbstractSubsystem::get_instance<Lift>();
    Intake& intake = AbstractSubsystem::get_instance<Intake>();
    Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();
    LadyBrown& lady_brown = AbstractSubsystem::get_instance<LadyBrown>();

    // Add commands to the chain here
    #if THINK
    add_command(make_unique<DriveStraight>(42, 1.5));
    add_command(make_unique<ToggleArm>());
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(250);})));
    add_command(make_unique<DriveStraight>(-36, 5));  // Pull first goal back
    add_command(make_unique<ToggleArm>());
    add_command(make_unique<DriveStraight>(6, 5));  // Unhook the first goal
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {
        drivetrain.set_reversing(true);
    })));
    add_command(make_unique<PurePursuit>(getGoal_txt, 12, 2));
    add_command(make_unique<ToggleClamp>());
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {
        drivetrain.set_reversing(false);
    })));
    add_command(make_unique<PPIntakeLift>(intake1_txt, 12, 2));
    // add_command(make_unique<DriveStraightIntakeLift>(-12, 3, 2));
    // add_command(make_unique<PurePursuit>(intakeCorner_txt, 12, 2));
    // add_command(make_unique<DriveStraight>(-8, 1, 2));
    // add_command(make_unique<TurnAround>());
    // add_command(make_unique<DriveStraight>(24, 1, 3));
    // add_command(make_unique<ToggleClamp>());
    #elif DO
t

    #endif
}

void AutonomousControlScheduler::initialize() {
    // drivetrain.initialize();
}

void AutonomousControlScheduler::shutdown() {
    drivetrain.set_braking(true);
}
