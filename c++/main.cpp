#include <iostream>
#include "lib/FindFormul.h"
#include "lib/RulesRealize.h"
#include <algorithm>

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& data) {
    for (auto& d : data)
        out << d << ' ';
    return out;
}

template<typename T>
void f(const std::vector<T>& data) {
    ;
}

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
    // Node axiom2 = Formula::to_expression_tree("((a > (b > c)) > ((a > b) > (a > c)))");
    // Node tmp;
    // Rule* mp = new ModusPonuns();
    // std::cout << mp->is_approp({axiom2, 2}, {axiom1, 1}, tmp);
    // std::cout << tmp << std::endl;
}
