#include "Command.hpp"

void Command::run() {
    /* Design note: there should not have to be secondary call to run() after a command is
     * initialized to run periodic() for the first time. There should also not be a second call
     * to run() after is_complete() == true to run shutdown(). */

    if (loops == 0) {
        initialize();
        periodic();
        goto end;
    }

    if (!completed) {
        periodic();

        if (is_complete()) {
            shutdown();
            completed = true;
        }

        goto end;
    }

    end:
    /* This values must be updated, unconditionally, at the end of the run() method. Previously,
     * we placed right after the call to periodic() in the if statement. This caused Instant
     * Command to run twice b/c they're always complete and never run periodic() */
    loops++;
}
