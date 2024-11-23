#include <iostream>
#include "lib/FindFormul.h"
#include "lib/RulesRealize.h"
#include <algorithm>

int main() {
    Node axiom1 = Formula::to_expression_tree("a > (b > a)");
    Node axiom2 = Formula::to_expression_tree("((a > (b > c)) > ((a > b) > (a > c)))");
    Node axiom3 = Formula::to_expression_tree("(!b > !a) > ((!b > a) > b)");
    Node axiom_tmp = Formula::to_expression_tree("(!a > !b) > (b > a)");

    FindFormul find_formul;
    find_formul.set_axioms({&axiom1, &axiom2, &axiom3, &axiom_tmp});

    ModusPonuns mp;
    find_formul.set_rules({&mp});

    Node formula = Formula::to_expression_tree("!(a > !b) > a");
    /*
    "!(a > !b) > a"
    "!(a > !b) > b"
    "a > (b > (a > !b))"
    "a > (!a > b)"
    "b > (!a > b)"
    "(a > c) > ((b > c) > ((!a > b) > c))"
    "!a > (a > b)"
    "a > a"
    */
    formula.set_all_const();
    find_formul.find_formul(formula);

    find_formul.clear_data();

    // std::cout << (Formula::to_expression_tree("(a > (!b > a))") == Formula::to_expression_tree("(b > (!a > b))"));

    // Node form1 = Formula::to_expression_tree("(a > (b > a))");
    // Node form2 = Formula::to_expression_tree("(!a > !b) > (!a > !b)");
    // Node new_node;
    // std::cout << *form1.left << std::endl;
    // ModusPonuns().is_approp({form1, 1}, {form2, 2}, new_node);
    // std::cout << new_node << std::endl;
}