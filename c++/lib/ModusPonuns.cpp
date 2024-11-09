#include "ModusPonuns.h"
#include <iostream>

bool unification(Node& first, Node& left, Node& right) {
    if (first.value != left.value && first.value.num_symbol == 0 && left.value.num_symbol == 0)
        return false;

    if (first.value.num_symbol == 0 && left.value.num_symbol != 0) {
        Formula::replace_chars(right, {Unifier(left, first)});
        Formula::to_zerros_num_value(right);
        return true;
    }
    return false;
}

bool ModusPonuns::is_approp(const std::pair<const Node&, int>& first_formula, const std::pair<const Node&, int>& second_formula, Node& new_formula) const {
    if (second_formula.first.value != '>')
        return false;
    
    Node first(first_formula.first, first_formula.second);
    Node left(*second_formula.first.left, second_formula.second);
    new_formula = Node(*second_formula.first.right, second_formula.second);

    return unification(first, left, new_formula);
}

std::string ModusPonuns::get_name() const { return "mp"; }
