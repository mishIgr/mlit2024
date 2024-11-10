#ifndef FORMULA_H
#define FORMULA_H

#include <string>
#include <iostream>
#include <vector>
#include "Node.h"
#include "Unifier.h"

class ErrorReadFormula : public std::exception {
public:
    const char* what() const noexcept override;
};

namespace Formula {
    Node to_expression_tree(const std::string& expression);
    std::vector<Unifier> replace_chars(Node& formula, const ValueNode& repl_value, const Node& replace_formula);
    std::vector<Unifier> replace_chars(Node& formula, const std::vector<Unifier>& data);
    std::vector<Unifier> to_zerros_num_value(Node& formula);
    std::pair<Node*, Node*> get_first_mismatched_pair(Node& first_formula, Node& second_formula);
    std::vector<Unifier> unification(Node& first, Node& left);
};

#endif
