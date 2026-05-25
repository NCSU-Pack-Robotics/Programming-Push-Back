#include "Pose.hpp"

#include <cmath>
#include <numeric>
#include <stdexcept>

double EuclideanPose::x() const {
    return position[0];
}

double EuclideanPose::y() const {
    return position[1];
}

EuclideanPose::EuclideanPose(const std::initializer_list<double> position) : position{position} {
    check_size();
}

EuclideanPose::EuclideanPose(const std::valarray<double>& position) : position{position} {
    check_size();
}

double EuclideanPose::distance(const EuclideanPose& other) const {
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

EuclideanPose EuclideanPose::operator+(const EuclideanPose& other) const {
    return {this->position + other.position};
}

EuclideanPose EuclideanPose::operator-(const EuclideanPose& other) const {
    return {this->position - other.position};
}

double EuclideanPose::operator*(const EuclideanPose& other) const {
    return std::inner_product(
        std::begin(this->position),
        std::end(this->position),
        std::begin(other.position),
        0.0);
}

EuclideanPose EuclideanPose::operator*(const double& s) const {
    return {this->position * s};
}

EuclideanPose EuclideanPose::operator/(const double& s) const {
    return {this->position / s};
}

EuclideanPose EuclideanPose::lerp(const EuclideanPose& other, const double t) const {
    return {this->position + (other.position - this->position) * t};
}

void EuclideanPose::check_size() const {
    if (position.size() < 2) {
        throw std::invalid_argument("A EuclideanPose must have at least 2 dimensions.");
    }
}

Pose::Pose(const double x, const double y, const double heading)
    : EuclideanPose({x, y}), heading(heading) {}

PathPose::PathPose(const double x, const double y, const double velocity)
    : EuclideanPose({x, y}), velocity(velocity) {}

