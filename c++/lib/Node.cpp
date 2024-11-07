#include "Node.h"
#include <utility>
#include <iostream>

Node::Node(char value, Node* left, Node* right)
    : value(value), left(left), right(right) {}

Node::Node(const Node& other) : value(other.value), left(nullptr), right(nullptr) {
    if (other.left)
        left = new Node(*other.left);
    if (other.right)
        right = new Node(*other.right);
}

Node::Node(Node&& other) noexcept : value(other.value), left(nullptr), right(nullptr) {
    std::swap(left, other.left);
    std::swap(right, other.right);
}

Node& Node::operator=(const Node& other) {
    if (this == &other)
        return *this;
    
    Node tmp(other);
    std::swap(value, tmp.value);
    std::swap(left, tmp.left);
    std::swap(right, tmp.right);
    return *this;
}

Node& Node::operator=(Node&& other) noexcept {
    if (this == &other)
        return *this;
    
    std::swap(value, other.value);
    std::swap(left, other.left);
    std::swap(right, other.right);
    return *this;
}

Node::~Node() {
    delete left;
    delete right;
}

bool Node::operator==(const Node& other) const {
    std::stack<Node*> stack_this;
    std::stack<Node*> stack_other;
    Node* this_ptr = new Node(value, left, right);
    Node* other_ptr = new Node(other.value, other.left, other.right);

    stack_this.push(this_ptr);
    stack_other.push(other_ptr);

    while (!stack_this.empty() && !stack_other.empty()) {
        Node* this_node = stack_this.top();
        Node* other_node = stack_other.top();
        stack_this.pop();
        stack_other.pop();

        if (this_node->value != other_node->value)
            return false;

        if (this_node->left && other_node->left) {
            stack_this.push(this_node->left);
            stack_other.push(other_node->left);
        } else if (this_node->left || other_node->left)
            return false; 

        if (this_node->right && other_node->right) {
            stack_this.push(this_node->right);
            stack_other.push(other_node->right);
        } else if (this_node->right || other_node->right)
            return false; 
    }

    this_ptr->left = nullptr;
    this_ptr->right = nullptr;
    delete this_ptr;
    other_ptr->left = nullptr;
    other_ptr->right = nullptr;
    delete other_ptr;

    return stack_this.empty() && stack_other.empty();
}

std::string Node::to_string() const {
    std::stack<const Node*> stack;
    stack.push(this);
    std::string output;
    
    while (!stack.empty()) {
        const Node* node = stack.top();
        stack.pop();
        output += node->value;

        if (node->right) stack.push(node->right);
        if (node->left) stack.push(node->left);
    }
    return output;
}

bool Node::operator<(const Node& other) const {
    if (value == other.value && left && other.left)
        return *left < *other.left;
    if (value == other.value && right && other.right)
        return *right < *other.right;
    return value < other.value;
}

std::ostream& operator<<(std::ostream& out, const Node& node) {
    if (node.right)
        out << '(' << *node.left << node.value << *node.right << ')';
    else if (node.left)
        out << node.value << *node.left;
    else
        out << node.value;
    return out;
}
