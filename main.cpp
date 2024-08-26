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
    std::cout << "Start" << std::endl;
    ctor_dtor();
    std::cout << "End" << std::endl;
    return 0;
}
