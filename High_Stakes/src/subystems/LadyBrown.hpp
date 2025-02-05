#pragma once
#include "../../include/main.h"
#include "../AbstractSubsystem.hpp"
#include "../Config.hpp"

class LadyBrown : public AbstractSubsystem {
    friend class AbstractSubsystem;

public:
    void initialize() override;

    void periodic() override;

    void disabled_periodic() override;

    void shutdown() override;

    void next_position();

private:
    /** The rotation sensor for the LadyBrown */
    std::unique_ptr<pros::Rotation> lb_sensor;

    /** The motor for the LadyBrown */
    std::unique_ptr<pros::Motor> lb_motor;

    /** The current position of the LadyBrown. It is not guaranteed to be in this position, but it will be attempting to get there */
    Constants::LadyBrown::Position position;

    /** The position index from the list of positions */
    int position_index;

    /** The PID used for the LadyBrown */
    PID lb_pid = PID(0,0,0);

protected:
    LadyBrown();
};