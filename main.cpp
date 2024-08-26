#include "kd-tree.hpp"
#include "rect.hpp"
#include "point.hpp"
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>

struct Test final {
    std::string name;
    void (*execute)(void);

    Test(const char* name, void (*execute)(void)): name{name}, execute{execute} {}
};

// Helper for verifying the contents of actual.
// Precondition: expected has no duplicates.
void assert_same_set(const std::vector<point_t>& actual, const std::vector<point_t>& expected) {
    assert(actual.size() == expected.size());
    for (point_t p : expected) {
        assert(std::find(actual.cbegin(), actual.cend(), p) != actual.cend());
    }
}

void test_empty_kdtree(void) {
    rect_t rect {0, 0, 100, 100};
    KdTree kd {rect, std::vector<point_t>{}};

    assert(!kd.contains(point_t{4, 5}));
    assert(!kd.contains(point_t{-20, -5}));
    assert(!kd.contains(point_t{50, 102}));
    assert(!kd.contains(point_t{60, 75}));

    assert(kd.rangeSearch(rect_t{-5, -5, 45, 45}).size() == 0);
    assert(kd.rangeSearch(rect_t{0, 0, 3, 3}).size() == 0);
    assert(kd.rangeSearch(rect_t{50, 50, 60, 100}).size() == 0);
    assert(kd.rangeSearch(rect_t{120, 120, 135, 150}).size() == 0);
}

void test_single_point_kdtree(void) {
    rect_t rect {0, 0, 10, 10};
    KdTree kd {rect, std::vector<point_t>{point_t{5, 6}}};

    assert(!kd.contains(point_t{3, 9}));
    assert(!kd.contains(point_t{11, 5}));
    assert(!kd.contains(point_t{8, -1}));

    assert(kd.contains(point_t{5, 6}));

    assert(kd.rangeSearch(rect_t{3, 3, 3, 3}).size() == 0);
    assert(kd.rangeSearch(rect_t{1, 0, 5, 6}).size() == 0);
    assert(kd.rangeSearch(rect_t{-1, -1, 4, 4}).size() == 0);

    assert_same_set(kd.rangeSearch(rect_t{5, 6, 1, 1}), std::vector<point_t>{point_t{5, 6}});
    assert_same_set(kd.rangeSearch(rect_t{2, 2, 6, 6}), std::vector<point_t>{point_t{5, 6}});
    assert_same_set(kd.rangeSearch(rect_t{5, 5, 4, 4}), std::vector<point_t>{point_t{5, 6}});
}

void test_multiple_points_kdtree_1(void) {
    rect_t rect {0, 0, 10, 10};
    KdTree kd {rect, std::vector<point_t>{
        point_t{2, 1},
        point_t{5, 3},
        point_t{1, 8},
        point_t{3, 6},
        point_t{9, 0},
        point_t{7, 7}
    }};

    assert(kd.contains(point_t{5, 3}));
    assert(kd.contains(point_t{7, 7}));
    assert(kd.contains(point_t{9, 0}));
    assert(kd.contains(point_t{1, 8}));

    assert(!kd.contains(point_t{1, 1}));
    assert(!kd.contains(point_t{8, 1}));
    assert(!kd.contains(point_t{6, 8}));
    assert(!kd.contains(point_t{3, 12}));

    assert_same_set(kd.rangeSearch(rect_t{1, 0, 4, 4}),
                    std::vector<point_t>{point_t{2, 1}});
    assert_same_set(kd.rangeSearch(rect_t{3, 3, 8, 5}),
                    std::vector<point_t>{point_t{5, 3}, point_t{3, 6}, point_t{7, 7}});
    assert_same_set(kd.rangeSearch(rect_t{-1, 5, 5, 4}),
                    std::vector<point_t>{point_t{3, 6}, point_t{1, 8}});
    assert_same_set(kd.rangeSearch(rect_t{1, -1, 10, 4}),
                    std::vector<point_t>{point_t{2, 1}, point_t{9, 0}});

    assert(kd.rangeSearch(rect_t{7, 3, 2, 3}).size() == 0);
    assert(kd.rangeSearch(rect_t{15, 5, 6, 6}).size() == 0);
    assert(kd.rangeSearch(rect_t{4, 10, 3, 3}).size() == 0);
}

void test_multiple_points_kdtree_2(void) {
    rect_t rect {0, 0, 6, 12};
    KdTree kd {rect, std::vector<point_t>{
        point_t{2, 1},
        point_t{4, 4},
        point_t{1, 5},
        point_t{3, 11},
        point_t{0, 10},
        point_t{5, 9}
    }};

    assert(kd.contains(point_t{4, 4}));
    assert(kd.contains(point_t{0, 10}));
    assert(kd.contains(point_t{3, 11}));
    assert(kd.contains(point_t{1, 5}));

    assert(!kd.contains(point_t{-3, -2}));
    assert(!kd.contains(point_t{5, 2}));
    assert(!kd.contains(point_t{3, 7}));
    assert(!kd.contains(point_t{0, 0}));

    assert_same_set(kd.rangeSearch(rect_t{0, 1, 5, 5}),
                    std::vector<point_t>{point_t{2, 1}, point_t{1, 5}, point_t{4, 4}});
    assert_same_set(kd.rangeSearch(rect_t{3, 8, 4, 5}),
                    std::vector<point_t>{point_t{5, 9}, point_t{3, 11}});
    assert_same_set(kd.rangeSearch(rect_t{0, 4, 4, 7}),
                    std::vector<point_t>{point_t{1, 5}, point_t{0, 10}});
    assert_same_set(kd.rangeSearch(rect_t{-2, -2, 9, 8}),
                    std::vector<point_t>{point_t{2, 1}, point_t{1, 5}, point_t{4, 4}});

    assert(kd.rangeSearch(rect_t{2, 6, 3, 3}).size() == 0);
    assert(kd.rangeSearch(rect_t{-1, 2, 3, 2}).size() == 0);
    assert(kd.rangeSearch(rect_t{-40, 40, 5, 5}).size() == 0);
}

int main(void) {
    std::vector<Test> tests;
    tests.emplace_back("test_empty_kdtree", &test_empty_kdtree);
    tests.emplace_back("test_single_point_kdtree", &test_single_point_kdtree);
    tests.emplace_back("test_multiple_points_kdtree_1", &test_multiple_points_kdtree_1);
    tests.emplace_back("test_multiple_points_kdtree_2", &test_multiple_points_kdtree_2);

    for (const Test& test : tests) {
        std::cout << "Running: " << test.name << std::endl;
        test.execute();
    }
    std::cout << "All tests pass!" << std::endl;
    return 0;
}
