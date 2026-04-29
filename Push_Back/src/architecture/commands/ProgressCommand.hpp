#pragma once

#include "Command.hpp"

/** A command that can have its progress tracked and used via <code>get_progress()</code> */
class ProgressCommand : public Command {

public:
    /**
     * Tracks and returns the current progress of the ProgressCommand
     * @return A value between 0 and 1 representing the progress of the command. 0: no progress & 1: complete
     */
    virtual double get_progress() = 0;
};
