#include "rect.hpp"

bool rect_t::contains(point_t point) const {
    return (point.x >= this->left) &&
           (point.y >= this->top) &&
           (point.x < this->left + this->width) &&
           (point.y < this->top + this->height);
}

bool rect_t::intersects(rect_t other) const {
    return (other.left < this->left + this->width) &&
           (other.top < this->top + this->height) &&
           (other.left + other.width > this->left) &&
           (other.top + other.height > this->top);
}
