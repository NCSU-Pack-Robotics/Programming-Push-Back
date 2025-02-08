#pragma once
#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../math/PID.hpp"
#include "../Config.hpp"

class LadyBrown : public AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

    void next_position();

    /** An enum that contains the possible positions of the LadyBrown */
    enum Position { STOW, LOAD, SCORE };

    /** A map of the positions and their rotation in centidegrees */
    std::array<std::pair<Position, int32_t>, 3> Positions{
        // std::pair{STOW, 500},
        // std::pair{LOAD, 4800},
        // std::pair{SCORE, 20500},
        std::pair{STOW, 15500},
        std::pair{LOAD, 19000},
        std::pair{SCORE, 34000},
    };

    /**
     * Gets the current enumeration <code>Position</code> of the LadyBrown.
     * @return The current position of the LadyBrown.
     */
    Position get_position() const;

private:
    /** The rotation sensor for the LadyBrown */
    std::unique_ptr<pros::Rotation> lb_sensor;

    /** The motor for the LadyBrown */
    std::unique_ptr<pros::Motor> lb_motor;

    /** The current position of the LadyBrown. It is not guaranteed to be in this position, but it will be attempting to get there */
    Position position;

    /** The position index from the list of positions */
    int position_index;

    /** The PID used for the LadyBrown */
    PID lb_pid = PID(Constants::PID::LadyBrown::Position::Kp,
        Constants::PID::LadyBrown::Position::Kd,
        Constants::PID::LadyBrown::Position::Ki);

protected:
    LadyBrown();
};