#include "Node.h"
#include <utility>
#include <iostream>
#include <queue>

ValueNode::ValueNode(char symbol, int num_symbol) : symbol(symbol), num_symbol(num_symbol) {}
ValueNode::ValueNode(char symbol) : symbol(symbol), num_symbol(0) {}
ValueNode::ValueNode() : symbol(0), num_symbol(0) {}

bool ValueNode::operator==(char symbol) const { return this->symbol == symbol; }
bool ValueNode::operator<(char symbol) const { return this->symbol < symbol; }
bool ValueNode::operator==(const ValueNode& other) const { return this->symbol == other.symbol && this->num_symbol == other.num_symbol; }
bool ValueNode::operator<(const ValueNode& other) const { return this->symbol < other.symbol; }

bool ValueNode::equal(const ValueNode& other) const { return symbol == other.symbol && num_symbol == other.num_symbol; }

std::ostream& operator<<(std::ostream& out, const ValueNode& node) {
    out << node.symbol;
    if (node.num_symbol)
        out << node.num_symbol;
    return out;
}

Node::Node(ValueNode value, Node* left, Node* right)
    : value(value), left(left), right(right) {}

Node::Node(char value, Node* left, Node* right)
    : value(value), left(left), right(right) {}

Node::Node(const Node& other, int num_symbol) : value(other.value), left(nullptr), right(nullptr) {
    if (other.left)
        left = new Node(*other.left, num_symbol);
    if (other.right)
        right = new Node(*other.right, num_symbol);
    if (other.left == nullptr && other.right == nullptr && value.num_symbol == 0)
        value.num_symbol = num_symbol;
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
