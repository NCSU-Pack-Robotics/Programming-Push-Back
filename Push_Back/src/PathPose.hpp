#pragma once
#include <string>

/**
 * Represents the pose of the robot.
 * In robotics, pose to the current state of a robot in the world.
 * The pose usually consists of the position and orientation of the robot.
 */
class PathPose {
public:
    /** The x-coordinate of the robot in inches. */
    double x;
    /** The y-coordinate of the robot in inches. */
    double y;
    /** The velocity of the robot in inches/second. */
    double velocity;

    /** Default constructor for the PathPose object. */
    PathPose() = default;

    /**
     * Constructs a new PathPose object.
     * @param x The x-coordinate in inches.
     * @param y The y-coordinate in inches.
     * @param velocity The velocity in inches/second.
     */
    PathPose(const double x, const double y, const double velocity) : x(x), y(y), velocity(velocity) {}

    /**
     * Gets the distance between two poses.
     * @param other The pose to get the distance to.
     * @return The distance between the two poses.
     */
    float distance(const PathPose& other) const;

    /**
     * Adds two poses to each-other.
     * Adding two poses will add their x and y values.
     * In space, this will look like adding two vectors.
     * The velocity value will not be modified. (It will be taken from this pose.)
     * @param other The pose to add to this pose.
     * @return The sum of the two poses.
     */
    PathPose operator+(const PathPose& other) const;

    /**
     * Subtracts two poses from each-other.
     * Subtracting two poses will subtract their x and y values.
     * In space, this will look like subtracting two vectors.
     * The velocity value will not be modified. (It will be taken from this pose.)
     * @param other The pose to subtract from this pose.
     * @return The difference of the two poses.
     */
    PathPose operator-(const PathPose& other) const;

    /**
     * Multiply two poses via dot product.
     * A dot product between two poses multiplies their x and y values and adds them together.
     * @param other
     * @return The dot product of the two poses.
     */
    float operator*(const PathPose& other) const;

    /**
     * Multiply a pose by a scalar value.
     * Multiplying a pose by a scalar will multiply the x and y values by the scalar value.
     * In space, this will look like scaling a vector up.
     * The velocity value will not be modified. (It will be taken from this pose.)
     * @param s
     * @return The pose multiplied by the scalar value.
     */
    PathPose operator*(const float& s) const;


    /**
     * Divides a pose by a scalar value.
     * Dividing a pose by a scalar will divide the x and y values by the scalar value.
     * In space, this will look like scaling a vector down.
     * The velocity value will not be modified. (It will be taken from this pose.)
     * @param s The scalar value to divide the pose by.
     * @return The pose divided by the scalar value.
     */
    PathPose operator/(const float& s) const;

    /**
     * Linearly interpolate between two poses.
     * The velocity is taken from this pose.
     * @param other The other pose to interpolate to.
     * @param t A scaling factor between 0 and 1 to how far between the two poses to interpolate.
     * @return The interpolated pose at t.
     */
    PathPose lerp(const PathPose& other, float t) const;

    /**
     * @return Returns the string representation of the pose.
     */
    std::string to_string() const;
};
