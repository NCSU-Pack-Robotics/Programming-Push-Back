#include <cmath>

#include "pose.hpp"

float Pose::distance(const Pose& other) const {
    return std::hypot(other.x - x, other.y - y);
}

Pose Pose::operator+(const Pose &other) const {
    return Pose(x + other.x, y + other.y, heading);
}

Pose Pose::operator-(const Pose& other) const {
    return Pose(x - other.x, y - other.y, heading);
}

float Pose::operator*(const Pose& other) const {
    return x * other.x + y * other.y;
}

Pose Pose::operator*(const float& s) const {
    return Pose(x * s, y * s, heading);
}

Pose Pose::operator/(const float& s) const {
    return Pose(x / s, y / s, heading);
}

Pose Pose::lerp(const Pose &other, const float t) const {
    return Pose(x + (other.x - x) * t, y + (other.y - y) * t, heading);
}
std::string Pose::to_string() const {
    // Convert heading to degrees
    const double heading = this->heading * 180 / M_PI;

    return "X: " + std::to_string(this->x) + " Y: " + std::to_string(this->y) + " Heading: " +
           std::to_string(heading);
}
