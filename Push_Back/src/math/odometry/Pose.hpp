#pragma once

#include <valarray>

/**
 * A point in n-dimensional Euclidean space.
 * Represents the pose of the robot.
 * In robotics, pose to the current state of a robot in the world.
 * The pose usually consists of the position and orientation of the robot.
 */
class EuclideanPose {
public:
    /** The position of the point in n-dimensional space.
     * In 2D space it is {x, y}, in 3D space it is {x, y, z}, etc. */
    std::valarray<double> position;

    EuclideanPose() = default;

    /** @return the first dimension of the position. In 2D space, this is the x-coordinate. */
    [[nodiscard]] double x() const;

    /** @return the second dimension of the position. In 2D space, this is the y-coordinate. */
    [[nodiscard]] double y() const;

    /**
     * Construct a Euclidean pose from a list of numbers.
     * @param position List of numbers as <code>{1, 2, 3, ...}</code>
     */
    EuclideanPose(std::initializer_list<double> position);

    /**
     * Construct a Euclidean pose from a valarray of numbers.
     * @param position List of numbers as <code>std::valarray<double>{1, 2, 3, ...}</code>
     */
    EuclideanPose(const std::valarray<double>& position);

    /**
     * Copy constructor.
     * @param other The Euclidean pose to copy.
     */
    EuclideanPose(const EuclideanPose& other) = default;

    /**
     * Compute the Euclidean distance between two Euclidean poses.
     * @param other The point to compute the distance to.
     * @return The distance Euclidean between the two points.
     */
    [[nodiscard]] double distance(const EuclideanPose& other) const;

    /**
     * @param other The other Euclidean pose to add to this one.
     * @return The of each dimension of the two Euclidean poses.
     */
    [[nodiscard]] EuclideanPose operator+(const EuclideanPose& other) const;

    /**
     * @param other The other Euclidean pose to subtract from this one.
     * @return The difference of each dimension of the two Euclidean poses.
     */
    [[nodiscard]] EuclideanPose operator-(const EuclideanPose& other) const;

    /**
     * @param other The other Euclidean pose to dot product with this one.
     * @return The dot product of the two Euclidean poses.
     */
    [[nodiscard]] double operator*(const EuclideanPose& other) const;

    /**
     * @param s The scalar value to multiply the Euclidean pose by.
     * @return The Euclidean pose with each dimension multiplied by the scalar value.
     */
    [[nodiscard]] EuclideanPose operator*(const double& s) const;

    /**
     * @param s The scalar value to divide the Euclidean pose by.
     * @return The Euclidean pose with each dimension divided by the scalar value.
     */
    [[nodiscard]] EuclideanPose operator/(const double& s) const;

    /**
     * Linearly interpolate between two Euclidean poses.
     * @param other The other Euclidean pose to interpolate to.
     * @param t A scaling factor between 0 and 1 to how far between the two Euclidean poses to interpolate.
     * @return The interpolated Euclidean pose at t.
     */
    [[nodiscard]] EuclideanPose lerp(const EuclideanPose& other, double t) const;

private:
    /**
     * Helper method for size checking on construction.
     * @throw std::invalid_argument if the size of the position is less than 2.
     */
    void check_size() const;
};

/**
 * A pose in 2D space.
 * Represents the pose of the robot.
 */
class Pose : public EuclideanPose{
public:
    /** The heading of the robot in radians. */
    double heading{};

    /** Default constructor for the Pose object. */
    Pose() = default;

    /**
     * Constructs a new Pose object.
     * @param x The x-coordinate in inches.
     * @param y The y-coordinate in inches.
     * @param heading The heading in radians.
     */
    Pose(double x, double y, double heading);
};

/**
 * A pose on a path in 2D space.
 * Represents a point on a path that the robot should follow.
 */
class PathPose : public EuclideanPose {
public:
    /** The speed at which the robot should be moving at this point in inches/second. */
    double velocity{};

    /** Default constructor for the PathPose object. */
    PathPose() = default;

    /*
     * Constructs a new PathPose object.
     * @param x The x-coordinate in inches.
     * @param y The y-coordinate in inches.
     * @param velocity The velocity in inches/second.
     */
    PathPose(double x, double y, double velocity);
};