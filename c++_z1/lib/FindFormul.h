#ifndef FIND_FORMUL_H
#define FIND_FORMUL_H

#include "Rule.h"
#include "Formula.h"
#include <vector>

class FindFormul {
    std::vector<Rule*> rules;
    std::vector<Node*> axioms;
    std::vector<Node*> data;
    std::map<Node*, std::pair<Node*, Node*>> pathMap; 
    std::vector<Node*> deduction_axiom;

public:
    FindFormul();
    void set_rules(const std::vector<Rule*>& rules);
    void set_axioms(const std::vector<Node*>& axioms);
    void clear_data();

    void recover_path(Node* find_node, std::ostream& out, int depth);
    void print_result(Node* find_node);

    void find_formul(const Node& formula);
};

#endif