#include "ModusPonuns.h"
#include <iostream>
#include <string>

bool ModusPonuns::is_approp(const std::pair<const Node&, int>& first_formula, const std::pair<const Node&, int>& second_formula, Node& new_formula) const {
    if (second_formula.first.value != '>')
        return false;
    
    Node first(first_formula.first, first_formula.second);
    Node left(*second_formula.first.left, second_formula.second);

    try {
        auto unifer = Formula::unification(first, left);
        if (unifer.size() == 0)
            return 0;
        new_formula = Node(*second_formula.first.right, second_formula.second);
        Formula::replace_chars(new_formula, unifer);
        auto un = Formula::to_zerros_num_value(new_formula);
        unifer.insert(unifer.end(), un.begin(), un.end());

        return true;
    } catch (ErrorCreateUnifier) {
        return false;
    } catch (const std::exception& e) {
        throw ;
    }
}

std::string ModusPonuns::get_name() const { return "mp"; }
