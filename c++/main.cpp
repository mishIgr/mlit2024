#include <iostream>
#include "lib/FindFormul.h"
#include "lib/RulesRealize.h"
#include <algorithm>

int main() {
    Node axiom1 = Formula::to_expression_tree("a > (b > a)");
    Node axiom2 = Formula::to_expression_tree("((a > (b > c)) > ((a > b) > (a > c)))");
    Node axiom3 = Formula::to_expression_tree("(!b > !a) > ((!b > a) > b)");

    FindFormul find_formul;
    find_formul.set_axioms({&axiom1, &axiom2, &axiom3});

    Rule* mp = new ModusPonuns();
    find_formul.set_rules({mp});

    find_formul.find_formul(Formula::to_expression_tree("!a > (a > b)"));

    find_formul.clear_data();

    // std::cout << (Formula::to_expression_tree("(!a>(a>a))") == Formula::to_expression_tree("(!a>(a>b))"));
}

/*
"!(a > !b) > a"
"!(a > !b) > b"
"a > (b > (a > !b))"
"a > (!a > b)"
"b > (!a > b)"
"(a > c) > ((b > c) > ((!a > b) > c))"
"!a > (a > b)"
"!a > !a"
*/