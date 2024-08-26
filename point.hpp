#ifndef POINT_HPP
#define POINT_HPP

struct point_t final {
    int x;
    int y;

    bool operator==(const point_t&) const;
    bool operator!=(const point_t&) const;
};

#endif
