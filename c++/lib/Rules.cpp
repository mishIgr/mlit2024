#include "Rules.h"
#include <iostream>

bool ModusPonuns::is_approp(const Node& first_formula, const Node& second_formula, Node& new_formula) const {
    if (first_formula.value == '!' || second_formula.value == '!')
        return false;
    if (first_formula.value == '>' && *first_formula.left == second_formula) {
        new_formula = *first_formula.right;
        return true;
    }
    if (second_formula.value == '>' && first_formula == *(second_formula.left)) {
        new_formula = *second_formula.right;
        return true;
    }
    return false;
}
