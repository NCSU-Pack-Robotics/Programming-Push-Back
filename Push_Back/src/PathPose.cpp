#include <cmath>

#include "PathPose.hpp"

float PathPose::distance(const PathPose& other) const {
    return std::hypot(other.x - x, other.y - y);
}

PathPose PathPose::operator+(const PathPose &other) const {
    return PathPose(x + other.x, y + other.y, velocity);
}

PathPose PathPose::operator-(const PathPose& other) const {
    return PathPose(x - other.x, y - other.y, velocity);
}

float PathPose::operator*(const PathPose& other) const {
    return x * other.x + y * other.y;
}

PathPose PathPose::operator*(const float& s) const {
    return PathPose(x * s, y * s, velocity);
}

PathPose PathPose::operator/(const float& s) const {
    return PathPose(x / s, y / s, velocity);
}

PathPose PathPose::lerp(const PathPose &other, const float t) const {
    return PathPose(x + (other.x - x) * t, y + (other.y - y) * t, velocity);
}
std::string PathPose::to_string() const {

    return "X: " + std::to_string(this->x) + " Y: " + std::to_string(this->y) + " Velocity: " +
           std::to_string(velocity);
}
