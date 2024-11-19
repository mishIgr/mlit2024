#include <iostream>
#include "lib/FindFormul.h"
#include "lib/RulesRealize.h"
#include <algorithm>

int main() {
    Node axiom1 = Formula::to_expression_tree("a > (b > a)");
    Node axiom2 = Formula::to_expression_tree("((a > (b > c)) > ((a > b) > (a > c)))");
    Node axiom3 = Formula::to_expression_tree("((!b > !a) > ((!b > a) > b))");

    FindFormul find_formul;
    find_formul.set_axioms({&axiom1, &axiom2, &axiom3});

    Rule* mp = new ModusPonuns();
    find_formul.set_rules({mp});

    find_formul.find_formul(Formula::to_expression_tree("a > a"));

    find_formul.clear_data();

    // Node axiom1 = Formula::to_expression_tree("a > (b > a)");
    // Node new_node;
    // ModusPonuns mp;
    // mp.is_approp({axiom1, 1}, {axiom1, 2}, new_node);
    // std::cout << new_node << std::endl;
}
