#ifndef DFS_H
#define DFS_H

#include <stack>
#include <iterator>
#include "Node.h"

template <typename NodePtr, typename = std::enable_if_t<std::is_same_v<std::remove_const_t<std::remove_pointer_t<NodePtr>>, Node>>>
class DepthFirstIterator : public std::iterator<std::forward_iterator_tag, NodePtr> {
    using NodeType = std::remove_pointer_t<NodePtr>;
    std::stack<NodePtr> stack_;

public:
    explicit DepthFirstIterator(NodePtr root) {
        if (root != nullptr)
            stack_.push(root);
    }

    decltype(auto) operator*() const { return *stack_.top(); }

    NodePtr operator->() const { return stack_.top(); }

    DepthFirstIterator& operator++() {
        NodePtr current = stack_.top();
        stack_.pop();

        if (current->right)
            stack_.push(current->right);
        if (current->left)
            stack_.push(current->left);

        return *this;
    }

    DepthFirstIterator operator++(int) {
        DepthFirstIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const DepthFirstIterator& other) const { return stack_ == other.stack_; }
    bool operator!=(const DepthFirstIterator& other) const { return !(*this == other); }
};

template <typename NodePtr, typename = std::enable_if_t<std::is_same_v<std::remove_const_t<std::remove_pointer_t<NodePtr>>, Node>>>
class DepthFirstIterable {
    NodePtr root;

public:
    explicit DepthFirstIterable(NodePtr root) : root(root) {}

    DepthFirstIterator<NodePtr> begin() { return DepthFirstIterator<NodePtr>(root); }
    DepthFirstIterator<NodePtr> end() { return DepthFirstIterator<NodePtr>(nullptr); }
};

template <typename NodePtr, typename = std::enable_if_t<std::is_same_v<std::remove_const_t<std::remove_pointer_t<NodePtr>>, Node>>>
using DFS = DepthFirstIterable<NodePtr>;

#endif
