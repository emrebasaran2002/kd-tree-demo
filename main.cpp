#include "kd-tree.hpp"
#include "rect.hpp"
#include "point.hpp"
#include <vector>
#include <iostream>

void ctor_dtor() {
    rect_t rect {0, 0, 100, 100};
    KdTree kd1 {rect, std::vector<point_t>{}};
    KdTree kd2 {rect, std::vector<point_t>{point_t{1, 2}}};
    KdTree kd3 {rect, std::vector<point_t>{point_t{1, 2}, point_t{3, 4}}};
    KdTree kd4 {rect, std::vector<point_t>{point_t{1, 2}, point_t{3, 4}, point_t{5, 3}, point_t{10, 10}}};
}

int main(void) {
    rect_t rect {0, 0, 10, 10};
    KdTree kd {rect, std::vector<point_t>{point_t{1, 2}, point_t{3, 4}, point_t{5, 3}, point_t{9, 9}}};

    std::cout << kd.contains(point_t{1, 2}) << std::endl; // 1
    std::cout << kd.contains(point_t{5, 3}) << std::endl; // 1
    std::cout << kd.contains(point_t{4, 3}) << std::endl; // 0
    std::cout << kd.contains(point_t{2, 2}) << std::endl; // 0

    // 1,2 and 3,4
    for (point_t p : kd.rangeSearch(rect_t {0, 0, 5, 5})) {
        std::cout << p.x << ',' << p.y << ' ';
    }
    std::cout << std::endl;

    // 1,2 and 5,3
    for (point_t p : kd.rangeSearch(rect_t {1, 1, 5, 3})) {
        std::cout << p.x << ',' << p.y << ' ';
    }
    std::cout << std::endl;

    return 0;
}
