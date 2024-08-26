#ifndef KD_TREE_HPP
#define KD_TREE_HPP

#include "point.hpp"
#include "rect.hpp"
#include <vector>

class KdTree final {
    private:
        struct Node final {
            bool isLeaf;
            rect_t rect;
            union {
                // Leaf-specific fields.
                point_t point;
                // Internal-specific fields.
                struct {
                    bool isHorizontal;
                    int limit;
                    Node* child_1;
                    Node* child_2;
                };
            };

            // Preconditions:
            // - begin/end must determine a valid, non-empty range.
            // - rect must contain all the points in the range.
            // - all points must have distinct x and y coordinates.
            Node(
                rect_t rect,
                std::vector<point_t>::iterator begin,
                std::vector<point_t>::iterator end,
                bool isHorizontal = false
            );

            Node(const Node& node);
            Node& operator=(const Node&) = delete;
            ~Node();

            // Precondition: this->rect contains point
            bool contains(point_t point) const;

            // Precondition: this->rect intersects rect
            void rangeSearch(rect_t rect, std::vector<point_t>& points) const;
        };

        Node* root;

    public:
        // Preconditions:
        // - all points must have distinct x coordinates.
        // - all points must have distinct y coordinates.
        // - rect must contain all points.
        KdTree(rect_t rect, std::vector<point_t> points);

        ~KdTree();

        KdTree(const KdTree& other);
        KdTree(KdTree&& other) noexcept;
        KdTree& operator=(const KdTree& other);
        KdTree& operator=(KdTree&& other) noexcept;

        bool contains(point_t point) const;
        std::vector<point_t> rangeSearch(rect_t rect) const;
};

#endif
