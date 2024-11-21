#ifndef BFS_H
#define BFS_H

#include <queue>
#include <iterator>
#include "Node.h"

template <typename NodePtr, typename = std::enable_if_t<std::is_same_v<std::remove_const_t<std::remove_pointer_t<NodePtr>>, Node>>>
class BreadthFirstIterator : public std::iterator<std::forward_iterator_tag, NodePtr> {
    using NodeType = std::remove_pointer_t<NodePtr>;
    std::queue<NodePtr> queue_;

public:
    explicit BreadthFirstIterator(NodePtr root) {
        if (root != nullptr)
            queue_.push(root);
    }

    decltype(auto) operator*() const { return *queue_.front(); }

    NodePtr operator->() const { return queue_.front(); }

    BreadthFirstIterator& operator++() {
        NodePtr current = queue_.front();
        queue_.pop();

        if (current->left)
            queue_.push(current->left);
        if (current->right)
            queue_.push(current->right);

        return *this;
    }

    BreadthFirstIterator operator++(int) {
        BreadthFirstIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const BreadthFirstIterator& other) const { return queue_ == other.queue_; }
    bool operator!=(const BreadthFirstIterator& other) const { return !(*this == other); }
};

template <typename NodePtr, typename = std::enable_if_t<std::is_same_v<std::remove_const_t<std::remove_pointer_t<NodePtr>>, Node>>>
class BreadthFirstIterable {
    NodePtr root;

public:
    explicit BreadthFirstIterable(NodePtr root) : root(root) {}

    BreadthFirstIterator<NodePtr> begin() { return BreadthFirstIterator<NodePtr>(root); }
    BreadthFirstIterator<NodePtr> end() { return BreadthFirstIterator<NodePtr>(nullptr); }
};

template <typename NodePtr, typename = std::enable_if_t<std::is_same_v<std::remove_const_t<std::remove_pointer_t<NodePtr>>, Node>>>
using BFS = BreadthFirstIterable<NodePtr>;

#endif