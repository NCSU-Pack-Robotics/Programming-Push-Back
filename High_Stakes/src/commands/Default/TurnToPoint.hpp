#pragma once

#include "TurnToHeading.hpp"

/**
 * This command turns the robot to a specified point using a point turn.
 * It uses the TurnToHeading command to turn to the heading of the point.
 */
class TurnToPoint : public TurnToHeading {
    /** The x coordinate of the point to which the robot will turn */
    double x;
    /** The y coordinate of the point to which the robot will turn */
    double y;

public:
    /**
     * Constructs a <code>TurnToPoint</code> command.
     * @param x The x coordinate of the point to which the robot will turn
     * @param y The y coordinate of the point to which the robot will turn
     * @param tolerance How close the robot can be to the target heading before finishing the
     * command (radians)
     */
    TurnToPoint(double x, double y, double tolerance);

    void initialize() override;
};
