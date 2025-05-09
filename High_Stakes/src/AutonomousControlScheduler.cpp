#include "AutonomousControlScheduler.hpp"

#include "asset.hpp"
#include "commands/Default/DumbDriveStraight.hpp"
#include "commands/Default/IntakeLiftTime.hpp"
#include "commands/Default/LiftTime.hpp"
#include "commands/Default/MoveLadyBrownPosition.hpp"
#include "commands/Default/TurnAround.hpp"
#include "commands/Default/TurnDegrees.hpp"
#include "commands/Default/TurnToHeading.hpp"
#include "commands/DriveStraight.hpp"
#include "commands/Instant/SetPose.hpp"
#include "commands/Instant/SetReversed.hpp"
#include "commands/Instant/ToggleArm.hpp"
#include "commands/Instant/ToggleClamp.hpp"
#include "commands/PPIntake.hpp"
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
ASSET(getGoal2_txt)
ASSET(intake1_txt)
ASSET(intakeCorner_txt)
ASSET(intake1Do_txt)
ASSET(intake2Do_txt)
ASSET(getGoalDo_txt)
ASSET(doinkTopRingDo_txt)
ASSET(intake1SimpleDo_txt)
ASSET(getGoalSimpleDo_txt)
ASSET(intake2SimpleDo_txt)
ASSET(intake3SimpleDo_txt)
ASSET(touchStakeSimpleDo_txt)

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
    add_command(make_unique<DriveStraight>(-34, 5));  // Pull first goal back
    add_command(make_unique<ToggleArm>());
    add_command(make_unique<DriveStraight>(6, 5));  // Unhook the first goal
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {
        drivetrain.set_reversing(true);
    })));
    add_command(make_unique<PurePursuit>(getGoal_txt, 16, 6));
    add_command(make_unique<ToggleClamp>());
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(300);})));
    add_command(make_unique<ToggleClamp>());
    add_command(make_unique<DumbDriveStraight>(20, 0.75));  // Backup to better align
    add_command(make_unique<ToggleClamp>());
    add_command(make_unique<LiftTime>(1));
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {
        drivetrain.set_reversing(false);
    })));
    add_command(make_unique<PPIntake>(intake1_txt, 12, 8));
    add_command(make_unique<DumbDriveStraight>(-30, 1.25));  // Push goal away
    add_command(make_unique<ToggleClamp>());
    add_command(make_unique<DumbDriveStraight>(30, 1.25));  // Move away from goal
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {
        drivetrain.set_reversing(true);
    })));
    add_command(make_unique<PPIntake>(getGoal2_txt, 8, 2));
    add_command(make_unique<ToggleClamp>());
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(300);})));
    add_command(make_unique<ToggleClamp>());
    add_command(make_unique<DumbDriveStraight>(20, 0.75));  // Backup to better align
    add_command(make_unique<ToggleClamp>());
    add_command(make_unique<IntakeLiftTime>(2, 127, 127, 1));
    add_command(make_unique<DumbDriveStraight>(-40, 1));


    #elif DO
    add_command(make_unique<DriveStraight>(40, 1.5, 3));
    add_command(make_unique<ToggleArm>());
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(250);})));
    add_command(make_unique<DriveStraight>(-36, 5));  // Pull first goal back
    add_command(make_unique<ToggleArm>());
    add_command(make_unique<DriveStraight>(6, 5));  // Unhook the first goal
    add_command(make_unique<PPIntake>(intake1SimpleDo_txt, 12, 2));
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {
        drivetrain.set_reversing(true);
    })));
    add_command(make_unique<PPIntake>(getGoalSimpleDo_txt, 8, 2));
    add_command(make_unique<ToggleClamp>());
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(300);})));
    add_command(make_unique<ToggleClamp>());
    add_command(make_unique<DumbDriveStraight>(20, 0.75));  // Backup to better align
    add_command(make_unique<ToggleClamp>());
    add_command(make_unique<IntakeTime>(1.5, Constants::Controller::MotorSpeeds::INTAKE_OUTWARDS));
    add_command(make_unique<LiftTime>(2));
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {drivetrain.set_reversing(false);})));
    add_command(make_unique<PPIntakeLift>(intake2SimpleDo_txt, 8.5, 2, 40));
    add_command(make_unique<ToggleClamp>());
    add_command(make_unique<PPIntake>(intake3SimpleDo_txt, 12, 11, 40));
    add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(500);})));
    add_command(make_unique<IntakeTime>(1, 127, 1));
    add_command(make_unique<TurnDegrees>(-82.5, 1, 1));
    add_command(make_unique<DriveStraight>(-5.75, 0.5, 2));
    add_command(make_unique<IntakeLiftTime>(3, 127, 127, 1));
    add_command(make_unique<MoveLadyBrownPosition>(LadyBrown::SCORE));
    add_command(make_unique<PurePursuit>(touchStakeSimpleDo_txt, 12, 5, 30));

    // add_command(make_unique<DriveStraight>(42, 1, 3));
    // add_command(make_unique<ToggleArm>());
    // add_command(make_unique<DriveStraight>(-48, 1.5, 2));
    // add_command(make_unique<ToggleArm>());
    // add_command(make_unique<DriveStraight>(6, 3, 2));
    // add_command(make_unique<DriveStraight>(-18, 2, 2));
    // add_command(make_unique<PPIntake>(intake1Do_txt, 8, 2, 56, 127));
    // add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(1000);})));
    // add_command(make_unique<DriveStraight>(-27, 1.5, 2));
    // add_command(make_unique<ToggleClamp>());
    // add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(300);})));
    // add_command(make_unique<ToggleClamp>());
    // add_command(make_unique<DumbDriveStraight>(-20, 0.75));  // Backup to better align
    // add_command(make_unique<ToggleClamp>());
    // add_command(make_unique<IntakeTime>(1, Constants::Controller::MotorSpeeds::INTAKE_OUTWARDS));
    // add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(300);})));
    // add_command(make_unique<LiftTime>(2));
    // add_command(make_unique<DriveStraightIntakeLift>(6, 1, 2));
    // add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(1000);})));
    // add_command(make_unique<PPIntake>(intake2Do_txt, 6, 2, 56, Constants::Controller::MotorSpeeds::INTAKE_OUTWARDS));

    // add_command(make_unique<PPIntake>(intake1Do_txt, 4, 2));
    // add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {drivetrain.set_reversing(true);})));
    // add_command(make_unique<PurePursuit>(getGoalDo_txt, 6, 2, 40));
    // add_command(make_unique<ToggleClamp>());
    // add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(300);})));
    // add_command(make_unique<ToggleClamp>());
    // add_command(make_unique<DumbDriveStraight>(15, 0.5));  // Backup to better align
    // add_command(make_unique<ToggleClamp>());
    // add_command(make_unique<IntakeTime>(1, Constants::Controller::MotorSpeeds::INTAKE_OUTWARDS));
    // add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {drivetrain.set_reversing(false);})));
    // add_command(make_unique<InstantCommand>(make_unique<function<void()>>([&] {pros::delay(300);})));
    // add_command(make_unique<PPIntakeLift>(doinkTopRingDo_txt, 10, 2, 40));  // Doink ring off to
    // add_command(make_unique<ToggleArm>());
    // add_command(make_unique<DriveStraight>(-14, 2, 2));  // Pull back top ring
    // add_command(make_unique<ToggleArm>());
    // add_command(make_unique<PPIntakeLift>(intake2Do_txt, 9, 18, 45));
    // add_command(make_unique<IntakeLiftTime>(2));
    // add_command(make_unique<IntakeTime>(1, Constants::Controller::MotorSpeeds::INTAKE_OUTWARDS));

    #endif
}

void AutonomousControlScheduler::initialize() {
    // drivetrain.initialize();
}

void AutonomousControlScheduler::shutdown() {
    drivetrain.set_braking(true);
}
