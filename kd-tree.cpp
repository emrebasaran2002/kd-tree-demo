#include "kd-tree.hpp"
#include <algorithm>

// ----------------------- Primary Ctor ----------------------- //

KdTree::KdTree(rect_t rect, std::vector<point_t> points) {
    if (points.size() == 0) {
        this->root = nullptr;
    } else {
        this->root = new Node {rect, points.begin(), points.end()};
    }
}

KdTree::Node::Node(
    rect_t rect,
    std::vector<point_t>::iterator begin,
    std::vector<point_t>::iterator end,
    bool isHorizontal
) {
    if (begin + 1 == end) {
        // Only one point, this is a leaf node.
        this->isLeaf = true;
        this->rect = rect;
        this->point = *begin;
    } else {
        // Two or more points, this is an internal node.
        this->isLeaf = false;
        this->rect = rect;
        this->isHorizontal = isHorizontal;
        // Partition the points. Could use quick-select, median-of-medians, etc.,
        // but for simplicity, we will just use std::sort.
        if (isHorizontal) {
            std::sort(begin, end, [](point_t p1, point_t p2) { return p1.y < p2.y; });
            size_t m = (end - begin) / 2;
            this->limit = (begin + m)->y;

            rect_t rect_1 {rect.left, rect.top, rect.width, this->limit - rect.top};
            this->child_1 = new Node {rect_1, begin, begin + m, false};

            rect_t rect_2 {rect.left, this->limit, rect.width, rect.height - rect_1.height};
            this->child_2 = new Node {rect_2, begin + m, end, false};
        } else {
            std::sort(begin, end, [](point_t p1, point_t p2) { return p1.x < p2.x; });
            size_t m = (end - begin) / 2;
            this->limit = (begin + m)->x;

            rect_t rect_1 {rect.left, rect.top, this->limit - rect.left, rect.height};
            this->child_1 = new Node {rect_1, begin, begin + m, true};

            rect_t rect_2 {this->limit, rect.top, rect.width - rect_1.width, rect.height};
            this->child_2 = new Node {rect_2, begin + m, end, true};
        }
    }
}

// ----------------------- Dtor ----------------------- //

KdTree::~KdTree() {
    delete this->root;
}

KdTree::Node::~Node() {
    if (!this->isLeaf) {
        delete this->child_1;
        delete this->child_2;
    }
}

// ----------------------- Copy ----------------------- //

KdTree::KdTree(const KdTree& other) {
    if (other.root == nullptr) {
        this->root = nullptr;
    } else {
        this->root = new Node {*other.root};
    }
}

KdTree& KdTree::operator=(const KdTree& other) {
    if (this != &other) {
        KdTree temp { other };
        std::swap(this->root, temp.root);
    }
    return *this;
}

KdTree::Node::Node(const Node& other) {
    this->isLeaf = other.isLeaf;
    this->rect = other.rect;
    if (this->isLeaf) {
        this->point = other.point;
    } else {
        this->isHorizontal = other.isHorizontal;
        this->limit = other.limit;
        this->child_1 = new Node { *other.child_1 };
        this->child_2 = new Node { *other.child_2 };
    }
}

// ----------------------- Move ----------------------- //

KdTree::KdTree(KdTree&& other) noexcept {
    this->root = other.root;
    other.root = nullptr;
}

KdTree& KdTree::operator=(KdTree&& other) noexcept {
    std::swap(this->root, other.root);
    return *this;
}

// ----------------------- Contains ----------------------- //

bool KdTree::contains(point_t point) const {
    if (this->root != nullptr && this->root->rect.contains(point)) {
        return this->root->contains(point);
    }
    return false;
}

bool KdTree::Node::contains(point_t point) const {
    if (this->isLeaf) {
        return this->point == point;
    } else {
        bool whichChild;
        if (this->isHorizontal) {
            whichChild = point.y < this->limit;
        } else {
            whichChild = point.x < this->limit;
        }
        if (whichChild) {
            return this->child_1->contains(point);
        } else {
            return this->child_2->contains(point);
        }
    }
}

// ----------------------- Range Search ----------------------- //

std::vector<point_t> KdTree::rangeSearch(rect_t rect) const {
    std::vector<point_t> points;
    if (this->root != nullptr && this->root->rect.intersects(rect)) {
        this->root->rangeSearch(rect, points);
    }
    return points;
}

void KdTree::Node::rangeSearch(rect_t rect, std::vector<point_t>& points) const {
    if (this->isLeaf) {
        if (rect.contains(this->point)) {
            points.push_back(this->point);
        }
    } else {
        if (this->child_1->rect.intersects(rect)) {
            this->child_1->rangeSearch(rect, points);
        }
        if (this->child_2->rect.intersects(rect)) {
            this->child_2->rangeSearch(rect, points);
        }
    }
}
