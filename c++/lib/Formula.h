#ifndef FORMULA_H
#define FORMULA_H

#include <string>
#include <unordered_map>
#include <stack>
#include <queue>
#include "Node.h"

class ErrorReadFormula : public std::exception {
public:
    const char* what() const noexcept override;
};

namespace Formula {
    Node to_expression_tree(const std::string& expression);
    Node replace_chars(const Node& formula, char repl_char, const Node& replace_formula);
};

#endif
