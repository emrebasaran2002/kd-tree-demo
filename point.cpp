#include "point.hpp"

bool point_t::operator==(const point_t& other) const {
    return this->x == other.x && this->y == other.y;
}

bool point_t::operator!=(const point_t& other) const {
    return !(*this == other);
}
