#include "Command.hpp"

void Command::run() {
    // On first loop, run initialization
    if (loops == 0)
        initialize();

    // Run periodic
    if (!is_complete()) {
        periodic();
    } else {  // If complete, finish
        shutdown();
    }

    /* This values must be updated, unconditionally, at the end of the run() method. Previously,
     * we placed right after the call to periodic() in the if statement. This caused Instant
     * Command to run twice b/c they're always complete and never run periodic() */
    loops++;
}
