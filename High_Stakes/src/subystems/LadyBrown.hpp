#pragma once
#include <array>

#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../Config.hpp"
#include "../math/PID.hpp"

class LadyBrown : public AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

    void next_position();

    /** An enum that contains the possible positions of the LadyBrown */
    enum Position { STOW, LOAD, ALIGN, SCORE };

    /** A map of the positions and their rotation in centidegrees */
    std::array<std::pair<Position, int32_t>, 4> Positions{
        #if THINK
        std::pair{STOW, 150},
        std::pair{LOAD, 2800},
        std::pair{ALIGN, 11100},
        std::pair{SCORE, 14000}
        #elif DO
        std::pair{STOW, 100},
        std::pair{LOAD, 3000},
        std::pair{ALIGN, 10900},
        std::pair{SCORE, 14500}
        #endif
    };

    /**
     * Gets the current target enumeration <code>Position</code> of the LadyBrown.
     * @return The current position of the LadyBrown.
     */
    Position get_position() const;

    /** Returns the last confirmed position of the LadyBrown */
    Position get_last_position() const;

     /**
     * Sets the target position of the LadyBrown.
     * @param position
     */
    void set_position(Position position);

    bool set_killed(bool killed);

private:
    /** The rotation sensor for the LadyBrown */
    std::unique_ptr<pros::Rotation> lb_sensor;

    /** The motor for the LadyBrown */
    std::unique_ptr<pros::Motor> lb_motor;

    /** This last position that the LadyBrown was in. If moving to a new position, until it is in that position this will be the last one */
    Position last_confirmed_position;

    /** The target position of the LadyBrown. It is not guaranteed to be in this position, but it will be attempting to get there */
    Position target_position;

    /** The position index from the list of positions */
    int position_index;

    /** A boolean that determines if all the motors should be off, otherwise the periodic sets the motors to the current position */
    bool motors_killed;

    /** The PID used for the LadyBrown */
    PID lb_pid = PID(Constants::PID::LadyBrown::Position::Kp,
        Constants::PID::LadyBrown::Position::Kd,
        Constants::PID::LadyBrown::Position::Ki);

protected:
    LadyBrown();
};