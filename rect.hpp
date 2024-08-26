#ifndef RECT_HPP
#define RECT_HPP

#include "point.hpp"

// A rect is said to contain its top and left edges, but not
// its right and bottom edges. The points where included and
// excluded edges intersect are also not included.
struct rect_t final {
    int left;
    int top;
    int width;
    int height;

    bool contains(point_t point) const;
    bool intersects(rect_t rect) const;
};

#endif
