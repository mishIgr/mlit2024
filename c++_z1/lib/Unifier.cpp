#include "Unifier.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>

const char* ErrorCreateUnifier::what() const noexcept { return "Ошибка создания унификатора"; }

Unifier::Unifier() {}
Unifier::Unifier(const Node& into, const Node& what) : into(into), what(what) {
    if (into.left || into.right || into.value.num_symbol == CONST_VALUE)
        throw ErrorCreateUnifier();

    std::queue<Node*> q;
    q.push(&this->what);

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        if (node->value.num_symbol == VARIBLE_VALUE && !node->left && !node->right) {
            if (node->value == into.value)
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
ValueNode Unifier::get_value_into() const { return into.value; }
ValueNode Unifier::get_value_what() const { return what.value; }

std::ostream& operator<<(std::ostream& out, const Unifier& other) {
    out << '{' << other.what << " | " << other.into << '}';
    return out;
}