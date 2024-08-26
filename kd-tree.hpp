#ifndef KD_TREE_HPP
#define KD_TREE_HPP

#include "point.hpp"
#include "rect.hpp"
#include <vector>

class KdTree final {
    private:
        struct Node;

        struct LeafNodeData final {
            point_t point;
        };
        struct InternalNodeData final {
            bool isHorizontal;
            int limit;
            Node* child_1;
            Node* child_2;
        };
        struct Node final {
            bool isLeaf;
            rect_t rect;
            union {
                LeafNodeData leafData;
                InternalNodeData InternalNodeData;
            };
        };

        Node* root;

    public:
        // Preconditions:
        // - points must not contain any duplicates
        // - rect must contain all points.
        KdTree(rect_t rect, const std::vector<point_t>& points);

        ~KdTree();

        // KdTree(const KdTree& other);
        // KdTree(KdTree&& other);
        // KdTree& operator=(const KdTree& other);
        // KdTree& operator=(KdTree&& other);

        // bool contains(point_t point) const;
        // std::vector<point_t> rangeSearch() const;
};

#endif
