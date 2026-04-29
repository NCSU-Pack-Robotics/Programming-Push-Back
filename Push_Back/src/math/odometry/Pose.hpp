#pragma once
#include <string>
#include <numeric>
#include <valarray>

/** A point in n-dimensional Euclidean space. */
class EuclideanPose {
    /** The position of the point in n-dimensional space.
     * In 2D space it is {x, y}, in 3D space it is {x, y, z}, etc. */
    std::valarray<double> position;

public:
    EuclideanPose() = default;

    /**
     * Construct a Euclidean pose from a list of numbers.
     * @param position List of numbers as <code>{1, 2, 3, ...}</code>
     */
    EuclideanPose(const std::initializer_list<double> position) : position{position} {}

    /**
     * Construct a Euclidean pose from a valarray of numbers.
     * @param position List of numbers as <code>std::valarray<double>{1, 2, 3, ...}</code>
     */
    EuclideanPose(const std::valarray<double>& position) : position{position} {}

    /**
     * Copy constructor.
     * @param other The Euclidean pose to copy.
     */
    EuclideanPose(const EuclideanPose& other) : position{other.position} {}

    /**
     * Compute the Euclidean distance between two Euclidean poses.
     * @param other The point to compute the distance to.
     * @return The distance Euclidean between the two points.
     */
    [[nodiscard]] double distance(const EuclideanPose& other) const {
        const auto pow_2 = [](const double a, const double b) {
            return std::pow(b - a, 2);
        };

        const double radicand = std::inner_product(
            std::begin(this->position),
            std::end(this->position),
            std::begin(other.position),
            0.0,
            std::plus(),
            pow_2);

        return std::sqrt(radicand);
    }

    /**
     * @param other The other Euclidean pose to add to this one.
     * @return The of each dimension of the two Euclidean poses.
     */
    [[nodiscard]] EuclideanPose operator+(const EuclideanPose& other) const {
        return {this->position + other.position};
    }

    /**
     * @param other The other Euclidean pose to subtract from this one.
     * @return The difference of each dimension of the two Euclidean poses.
     */
    [[nodiscard]] EuclideanPose operator-(const EuclideanPose& other) const {
        return {this->position - other.position};
    }

    /**
     * @param other The other Euclidean pose to dot product with this one.
     * @return The dot product of the two Euclidean poses.
     */
    [[nodiscard]] double operator*(const EuclideanPose& other) const {
        return std::inner_product(
            std::begin(this->position),
            std::end(this->position),
            std::begin(other.position),
            0.0);
    }

    /**
     * @param s The scalar value to multiply the Euclidean pose by.
     * @return The Euclidean pose with each dimension multiplied by the scalar value.
     */
    [[nodiscard]] EuclideanPose operator*(const double& s) const {
        return {this->position * s};
    }

    /**
     * @param s The scalar value to divide the Euclidean pose by.
     * @return The Euclidean pose with each dimension divided by the scalar value.
     */
    [[nodiscard]] EuclideanPose operator/(const double& s) const {
        return {this->position / s};
    }

    /**
     * Linearly interpolate between two Euclidean poses.
     * @param other The other Euclidean pose to interpolate to.
     * @param t A scaling factor between 0 and 1 to how far between the two Euclidean poses to interpolate.
     * @return The interpolated Euclidean pose at t.
     */
    [[nodiscard]] EuclideanPose lerp(const EuclideanPose& other, const double t) const {
        return {this->position + (other.position - this->position) * t};
    }
};

/**
 * Represents the pose of the robot.
 * In robotics, pose to the current state of a robot in the world.
 * The pose usually consists of the position and orientation of the robot.
 */
class Pose {
public:
    /** The x-coordinate of the robot in inches. */
    double x;
    /** The y-coordinate of the robot in inches. */
    double y;
    /** The heading of the robot in radians. */
    double heading;

    /** Default constructor for the Pose object. */
    Pose() = default;

    /**
     * Constructs a new Pose object.
     * @param x The x-coordinate in inches.
     * @param y The y-coordinate in inches.
     * @param heading The heading in radians.
     */
    Pose(const double x, const double y, const double heading) : x(x), y(y), heading(heading) {}

    /**
     * Gets the distance between two poses.
     * @param other The pose to get the distance to.
     * @return The distance between the two poses.
     */
    [[nodiscard]] float distance(const Pose& other) const;

    /**
     * Adds two poses to each-other.
     * Adding two poses will add their x and y values.
     * In space, this will look like adding two vectors.
     * The heading value will not be modified. (It will be taken from this pose.)
     * @param other The pose to add to this pose.
     * @return The sum of the two poses.
     */
    Pose operator+(const Pose& other) const;

    /**
     * Subtracts two poses from each-other.
     * Subtracting two poses will subtract their x and y values.
     * In space, this will look like subtracting two vectors.
     * The heading value will not be modified. (It will be taken from this pose.)
     * @param other The pose to subtract from this pose.
     * @return The difference of the two poses.
     */
    Pose operator-(const Pose& other) const;

    /**
     * Multiply two poses via dot product.
     * A dot product between two poses multiplies their x and y values and adds them together.
     * @param other
     * @return The dot product of the two poses.
     */
    float operator*(const Pose& other) const;

    /**
     * Multiply a pose by a scalar value.
     * Multiplying a pose by a scalar will multiply the x and y values by the scalar value.
     * In space, this will look like scaling a vector up.
     * The heading value will not be modified. (It will be taken from this pose.)
     * @param s
     * @return The pose multiplied by the scalar value.
     */
    Pose operator*(const float& s) const;


    /**
     * Divides a pose by a scalar value.
     * Dividing a pose by a scalar will divide the x and y values by the scalar value.
     * In space, this will look like scaling a vector down.
     * The heading value will not be modified. (It will be taken from this pose.)
     * @param s The scalar value to divide the pose by.
     * @return The pose divided by the scalar value.
     */
    Pose operator/(const float& s) const;

    /**
     * Linearly interpolate between two poses.
     * The heading is taken from this pose.
     * @param other The other pose to interpolate to.
     * @param t A scaling factor between 0 and 1 to how far between the two poses to interpolate.
     * @return The interpolated pose at t.
     */
    [[nodiscard]] Pose lerp(const Pose& other, float t) const;

    /**
     * @return Returns the string representation of the pose.
     */
    [[nodiscard]] std::string to_string() const;
};
