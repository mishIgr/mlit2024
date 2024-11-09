#ifndef FORMULA_H
#define FORMULA_H

#include <string>
#include <unordered_map>
#include <stack>
#include <queue>
#include <iostream>
#include "Node.h"

class ErrorReadFormula : public std::exception {
public:
    const char* what() const noexcept override;
};

class Unifier {
    Node into;
    Node what;

public:
    Unifier();
    Unifier(const Node& into, const Node& what);

    bool equal_into(const Node& into) const;
    bool equal_what(const Node& what) const;
    Node get_into() const;
    Node get_what() const;

    friend std::ostream& operator<<(std::ostream& out, const Unifier& other);
};

namespace Formula {
    Node to_expression_tree(const std::string& expression);
    std::vector<Unifier> replace_chars(Node& formula, const ValueNode& repl_value, const Node& replace_formula);
    void replace_chars(Node& formula, const std::vector<Unifier>& data);
    std::vector<Unifier> to_zerros_num_value(Node& formula);
};

#endif
