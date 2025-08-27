
#pragma once

#include "../../Command.hpp"
#include "../../Timer.hpp"
#include "../../subystems/Lift.hpp"
#include "../../Config.hpp"

/** Intakes for a specified amount of time. */
class LiftTime : public Command {
    /** The time to intake for in seconds. */
    double time;
    /** The direction to intake in. 1 for inwards, -1 for outwards. */
    double direction;
    /** The speed of the intake. */
    double speed;

    /** The timer used to track the time. */
    Timer timer;

    /** A reference to the intake subsystem. */
    Lift& lift = AbstractSubsystem::get_instance<Lift>();

public:
    /**
     * Constructor for the IntakeTime command.
     * @param time The time to intake for in seconds.
     * @param direction 1 for inwards, -1 for outwards.
     */
    LiftTime(double time, double speed = Constants::Controller::MotorSpeeds::INTAKE_INWARDS, double direction = 1);

    void initialize() override;
    void periodic() override;
    void shutdown() override;
    bool is_complete() override;

};