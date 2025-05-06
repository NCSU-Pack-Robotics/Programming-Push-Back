#include "AutonomousControlScheduler.hpp"

#include "asset.hpp"
#include "commands/Default/MoveLadyBrownPosition.hpp"
#include "commands/Default/TurnAround.hpp"
#include "commands/DriveStraight.hpp"
#include "commands/Instant/SetPose.hpp"
#include "commands/Instant/SetReversed.hpp"
#include "commands/Instant/ToggleClamp.hpp"
#include "commands/Instant/ToggleArm.hpp"
#include "commands/PPIntakeLift.hpp"
#include "commands/PurePursuit.hpp"
#include "subystems/Clamp.hpp"
#include "subystems/LadyBrown.hpp"
#include "subystems/Lift.hpp"

// Global paths defined here
ASSET(rushCenterFromLeft_txt)
ASSET(alignForGoal_txt)
ASSET(getGoal_txt)
ASSET(intake1_txt)

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
    add_command(make_unique<DriveStraight>(-25, 5, 3));  // Slam into the wall
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {drivetrain.set_pose(Pose(133.41, 43.38, M_PI)); })));  // Reset pose to match the wall
    add_command(make_unique<PurePursuit>(alignForGoal_txt, 3.5));
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {
        drivetrain.set_reversing(true);
    })));
    add_command(make_unique<PurePursuit>(getGoal_txt, 18, 2));
    add_command(make_unique<ToggleClamp>());
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {
        drivetrain.set_reversing(false);
    })));
    add_command(make_unique<PPIntakeLift>(intake1_txt, 3, 4));
    #elif DO
t

    #endif
}

void AutonomousControlScheduler::initialize() {
    drivetrain.initialize();
}

void AutonomousControlScheduler::shutdown() {
    drivetrain.set_braking(true);
}
