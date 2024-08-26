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

            // Precondition: begin/end must be a valid, non-empty range.
            Node(
                rect_t rect,
                std::vector<point_t>::iterator begin,
                std::vector<point_t>::iterator end,
                bool isHorizontal = false
            );

            Node(const Node& node);
            ~Node();

            Node& operator=(const Node&) = delete;
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
