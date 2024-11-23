#include <iostream>
#include "lib/FindFormul.h"
#include "lib/RulesRealize.h"
#include <algorithm>

int main() {
    FindFormul find_formul;

    ModusPonuns mp;
    find_formul.set_rules({ &mp });

    Node axiom1 = Formula::to_expression_tree("a > (b > a)");
    Node axiom2 = Formula::to_expression_tree("((a > (b > c)) > ((a > b) > (a > c)))");
    Node axiom3 = Formula::to_expression_tree("(!b > !a) > ((!b > a) > b)");
    Node new_axiom1 = Formula::to_expression_tree("(!a > !b) > (b > a)");
    Node new_axiom2 = Formula::to_expression_tree("a");

    /*
    "!(a > !b) > a"                          new_axiom1   4
    "!(a > !b) > b"                          new_axiom1   5
    "a > (b > (a > !b))"                     ...          6
    "a > (!a > b)"                           new_axiom1   7
    "b > (!a > b)"                           solo         8
    "(a > c) > ((b > c) > ((!a > b) > c))"   ...          9
    "!a > (a > b)"                           new_axiom1  10
    "a > a"                                  solo        11
    */

    Node axiom4 = Formula::to_expression_tree("!(a > !b) > a");
    Node axiom5 = Formula::to_expression_tree("!(a > !b) > b");
    Node axiom6 = Formula::to_expression_tree("a > (b > (a > !b))");
    Node axiom7 = Formula::to_expression_tree("a > (!a > b)");
    Node axiom8 = Formula::to_expression_tree("b > (!a > b)");
    Node axiom9 = Formula::to_expression_tree("(a > c) > ((b > c) > ((!a > b) > c))");
    Node axiom10 = Formula::to_expression_tree("!a > (a > b)");
    Node axiom11 = Formula::to_expression_tree("a > a");

    find_formul.set_axioms({ &axiom1, &axiom2, &axiom3, &new_axiom1});

    Node formula = axiom11;
    formula.set_all_const();
    find_formul.find_formul(formula);

    find_formul.clear_data();
}