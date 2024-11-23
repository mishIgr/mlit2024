#include "Node.h"
#include <utility>
#include <iostream>
#include <queue>
#include <map>

#define FIRST_VALUE 'a'
#define COLOR_BLUE "\033[96m"
#define COLOR_STANDART "\033[39m"

ValueNode::ValueNode(char symbol, int num_symbol) : symbol(symbol), num_symbol(num_symbol) {}
ValueNode::ValueNode(char symbol) : symbol(symbol), num_symbol(VARIBLE_VALUE) {}
ValueNode::ValueNode() : symbol(0), num_symbol(VARIBLE_VALUE) {}

bool ValueNode::operator==(char symbol) const { return this->symbol == symbol; }
bool ValueNode::operator<(char symbol) const { return this->symbol < symbol; }
bool ValueNode::operator==(const ValueNode& other) const { return symbol == other.symbol && num_symbol == other.num_symbol; }
bool ValueNode::operator<(const ValueNode& other) const { return symbol != other.symbol ? symbol < other.symbol : num_symbol < other.num_symbol; }

bool ValueNode::is_varible_or_const() const { return num_symbol == CONST_VALUE || num_symbol == VARIBLE_VALUE; }

std::ostream& operator<<(std::ostream& out, const ValueNode& node) {
    out << node.symbol;
    if (node.num_symbol != VARIBLE_VALUE && node.num_symbol != CONST_VALUE)
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
    if (other.left == nullptr && other.right == nullptr && value.num_symbol == VARIBLE_VALUE)
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

bool is_equal(std::map<ValueNode, ValueNode>& data, const ValueNode& key1, const ValueNode& key2, char& next_value) {
    if (key1.num_symbol == CONST_VALUE && key2.num_symbol == CONST_VALUE && key1 != key2)
        return false;

    auto it1 = data.find(key1);
    auto it2 = data.find(key2);

    if (it1 != data.end() && it2 != data.end())
        return it1->second == it2->second;

    if (it1 != data.end() || it2 != data.end())
        return false;

    data[key1] = next_value;
    data[key2] = next_value;
    next_value++;
    return true;
}

bool Node::operator==(const Node& other) const {
    std::stack<Node*> stack_this;
    std::stack<Node*> stack_other;
    std::map<ValueNode, ValueNode> replace_data;
    char next_value = FIRST_VALUE;

    Node unif_this = Node(*this, 1);
    Node unif_other = Node(other, 2);

    stack_this.push(&unif_this);
    stack_other.push(&unif_other);

    while (!stack_this.empty() && !stack_other.empty()) {
        Node* this_node = stack_this.top();
        Node* other_node = stack_other.top();
        stack_this.pop();
        stack_other.pop();


        if (!is_equal(replace_data, this_node->value, other_node->value, next_value))
            return false;

        if (this_node->right && other_node->right) {
            stack_this.push(this_node->right);
            stack_other.push(other_node->right);
        } else if (this_node->right || other_node->right)
            return false; 

        if (this_node->left && other_node->left) {
            stack_this.push(this_node->left);
            stack_other.push(other_node->left);
        } else if (this_node->left || other_node->left)
            return false;
    }

    return stack_this.empty() && stack_other.empty();
}

bool Node::operator<(const Node& other) const {
    if (value == other.value && left && other.left)
        return *left < *other.left;
    if (value == other.value && right && other.right)
        return *right < *other.right;
    return value < other.value;
}

void Node::set_all_const() {
    if (this->left) this->left->set_all_const();
    if (this->right) this->right->set_all_const();

    if (!this->left && !this->right)
        value.num_symbol = CONST_VALUE;
}

void Node::set_all_varible() {
    if (this->left) this->left->set_all_varible();
    if (this->right) this->right->set_all_varible();

    if (!this->left && !this->right)
        value.num_symbol = VARIBLE_VALUE;
}

bool Node::equal(const Node& other) const {
    std::stack<const Node*> stack_this;
    std::stack<const Node*> stack_other;

    stack_this.push(this);
    stack_other.push(&other);

    while (!stack_this.empty() && !stack_other.empty()) {
        const Node* this_node = stack_this.top();
        const Node* other_node = stack_other.top();
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

    return stack_this.empty() && stack_other.empty();
}

std::ostream& operator<<(std::ostream& out, const Node& node) {
    if (node.right)
        out << '(' << *node.left << ' ' << node.value << ' ' << *node.right << ')';
    else if (node.left)
        out << node.value << *node.left;
    else
        out << ((node.value.num_symbol == CONST_VALUE) ? COLOR_BLUE : COLOR_STANDART) << node.value << COLOR_STANDART;
    return out;
}
