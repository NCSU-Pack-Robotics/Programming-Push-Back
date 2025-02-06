#pragma once
#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
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
    enum Position { STOW, LOAD, SCORE };

    /** A map of the positions and their rotation in centidegrees */
    std::array<std::pair<Position, int32_t>, 3> Positions{
        std::pair{Position::STOW, 500},
        std::pair{Position::LOAD, 4800},
        std::pair{Position::SCORE, 20500},
    };

    Position get_position();

    

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
    PID lb_pid = PID(2.4,0,0.1);

protected:
    LadyBrown();
};