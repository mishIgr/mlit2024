#ifndef FORMULA_H
#define FORMULA_H

#include "Node.h"
#include "Unifier.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>

#define FIRST_VALUE 'a'

class ErrorReadFormula : public std::exception {
public:
    const char* what() const noexcept override;
};


class ReplaceParam {
    char next_value;

    ValueNode get_or_default(std::map<ValueNode, ValueNode>& data, const ValueNode& key);

public:
    static char find_next_value(Node* formula);

    ReplaceParam(char next_value = FIRST_VALUE);

    void to_zerros_num_value(Node* formula, std::map<ValueNode, ValueNode>& replace_data, std::vector<Unifier>& unif_data);
    void change_form_to_var(Node* formula, const std::vector<Unifier>& unif_data);
    void del_repet_denial(Node* formula);
};

namespace Formula {
    Node to_expression_tree(const std::string& expression);
    bool unification(std::pair<const Node&, int> first_formula, std::pair<const Node&, int> second_formula, std::vector<Unifier>& unif_data);
};

#endif
