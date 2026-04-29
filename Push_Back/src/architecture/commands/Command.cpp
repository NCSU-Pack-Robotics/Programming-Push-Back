#include "Command.hpp"

void Command::run() {
    if (calls++ == 0) {
        initialize();
    } else if (!completed) {
        periodic();

        if (is_complete()) {
            shutdown();
            completed = true;
        }
    }
}

bool Command::has_shutdown() const {
    return completed;
}
