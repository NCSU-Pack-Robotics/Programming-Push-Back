#include "../include/main.h"
#include "../include/pros/apix.h"

#include <cstring>
#include <fcntl.h>
#include <thread>
#include <stdfloat>

#include "AutonomousControlScheduler.hpp"
#include "DriverControlScheduler.hpp"
#include "subsystems/Drivetrain.hpp"

#include "SerialHandler.hpp"
#include "packets/OpticalPacket.hpp"
#include "packets/InitializeOpticalPacket.hpp"
#include "packets/InitializeOpticalCompletePacket.hpp"


// Turn off pros banner. Seems to only work in the macro version
ENABLE_BANNER(false)

void pi_communication();
pros::Task communication_task(pi_communication);

// Create all subsystems:
Drivetrain& drivetrain = AbstractSubsystem::get_instance<Drivetrain>();

// Add subsystems to vector for iteration
std::vector<AbstractSubsystem*> subsystems = { &drivetrain };

SerialHandler serial_handler;

void pi_communication()
{
    // Disable pros COBS which seems to include the sout/serr prefixes
    pros::c::serctl(SERCTL_DISABLE_COBS, nullptr);

    // Send a single null byte to be a delimiter between pros/vex junk bytes and our data
    // With cobs off there's actually no bytes sent, but good to have just in case
    fwrite("", 1, 1, stdout);

    serial_handler.add_listener<InitializeOpticalCompletePacket>([](SerialHandler& serial_handler, const Packet& packet) {

    });

    serial_handler.add_listener<OpticalPacket>([](SerialHandler& serial_handler, const Packet& packet) {

    });

    serial_handler.send(InitializeOpticalPacket{});

    while (true)
    {
        serial_handler.receive();
    }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

    // debug = true;
    // Initialize all subsystems
    for (AbstractSubsystem* subsystem : subsystems) {
        subsystem->initialize();
    }
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
    // Ensure all subsystems have shutdown/stopped.
    for (AbstractSubsystem* subsystem : subsystems) {
        subsystem->shutdown();
    }

    // Run disabled periodic for all subsystems
    while (true) {
        for (AbstractSubsystem* subsystem: subsystems) {
            subsystem->disabled_periodic();
        }
    }
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
    // Initialize the autonomous scheduler
    AutonomousControlScheduler autonomous_scheduler{};

    autonomous_scheduler.initialize();

    // TODO: Tell pi we have entered autonomous

    // Run forever
    while (true) {
        // Run the autonomous scheduler to do our routine
        autonomous_scheduler.run();

        // Run periodic for all subsystems
        for (AbstractSubsystem* subsystem : subsystems) {
            subsystem->periodic();
        }

        // Delay the loop to prevent the CPU from being overwhelmed
        pros::delay(5);
    }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
    // Initialize the driver control scheduler
    DriverControlScheduler driver_scheduler{};
    driver_scheduler.initialize();

    // TODO: Tell pi we have entered opcontrol

    while (true) {
        std::optional<Packet> packet = serial_handler.pop_latest<OpticalPacket>();
        // Print received data or 0 if not received. As long as PI sends faster than this loop delay this should work for this test
        // TODO: Test it
        std::float64_t x{}, y{}, heading{};
        if (packet.has_value())
        {
            x = packet->get_data<OpticalPacket>().x;
            y = packet->get_data<OpticalPacket>().y;
            heading = packet->get_data<OpticalPacket>().heading;
        }
        pros::c::screen_print_at(TEXT_LARGE, 0, 0, std::format("{:.2f} {:.2f} {:.2f}", x, y, heading).c_str());
        driver_scheduler.run();

        // Run periodic for all subsystems
        for (AbstractSubsystem* subsystem : subsystems) {
            subsystem->periodic();
        }

        // Delay the loop to prevent the CPU from being overwhelmed
        pros::delay(5);
    }
}
