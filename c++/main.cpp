#include <iostream>
#include "lib/FindFormul.h"
#include <algorithm>

int main() {
    Node axiom1 = Formula::to_expression_tree("a > (b > a)");
    Node axiom2 = Formula::to_expression_tree("((a > (b > c)) > ((a > b) > (a > c)))");
    Node axiom3 = Formula::to_expression_tree("((!b > !a) > ((!b > a) > b))");

    FindFormul find_formul;
    find_formul.set_axioms({&axiom1, &axiom2, &axiom3});

    Rules* mp = new ModusPonuns();
    find_formul.set_rules({mp});

    std::vector<Node> simple_substitutions{
        Formula::to_expression_tree("a"), 
        Formula::to_expression_tree("b"), 
        Formula::to_expression_tree("c"), 
        Formula::to_expression_tree("a>b"), 
        Formula::to_expression_tree("b>a"),
        Formula::to_expression_tree("!(a>!b)"), 
        Formula::to_expression_tree("!(b>!a)"), 
        Formula::to_expression_tree("!a>b"), 
        Formula::to_expression_tree("a>c"),
        Formula::to_expression_tree("a > (b > a)"),
        Formula::to_expression_tree("((a > (b > c)) > ((a > b) > (a > c)))"),
        Formula::to_expression_tree("((!b > !a) > ((!b > a) > b))")
    };

    std::vector<Node*> simple_substitution_ptrs;
    for (auto& simple_substitution : simple_substitutions)
        simple_substitution_ptrs.push_back(&simple_substitution);
    
    find_formul.set_simple_substitutions(simple_substitution_ptrs);

    find_formul.set_permis_chars("abc");

    find_formul.find_formul(Formula::to_expression_tree("!(a > !b) > a"));

    find_formul.clear_data();
}
