#include "Unifier.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>

const char* ErrorCreateUnifier::what() const noexcept {
    return "Невозможно в формулу подставить себя";
}

Unifier::Unifier() {}
Unifier::Unifier(const Node& into, const Node& what) : into(into), what(what) {
    std::vector<ValueNode> data;
    std::queue<Node*> q;
    q.push(&this->into);

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        if (node->value.num_symbol == 0 && !node->left && !node->right)
            data.push_back(node->value);

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }

    q.push(&this->what);

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        if (node->value.num_symbol == 0 && !node->left && !node->right) {
            auto it = std::find_if(data.begin(), data.end(), [node](ValueNode value_node) {
                return node->value == value_node;
            });
            if (it != data.end())
                throw ErrorCreateUnifier();
        }

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}

bool Unifier::equal_into(const Node& into) const { return this->into == into; }
bool Unifier::equal_what(const Node& what) const { return this->what == what; }
Node Unifier::get_into() const { return Node(into); }
Node Unifier::get_what() const { return Node(what); }

std::ostream& operator<<(std::ostream& out, const Unifier& other) {
    out << '{' << other.what << " | " << other.into << '}';
    return out;
}