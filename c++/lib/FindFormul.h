#ifndef FIND_FORMUL_H
#define FIND_FORMUL_H

#include "Rule.h"
#include "Formula.h"
#include <vector>

class FindFormul {
    std::vector<Rule*> rules;
    std::vector<Node*> axioms;
    std::vector<Node*> data;

public:
    FindFormul();
    void set_rules(const std::vector<Rule*>& rules);
    void set_axioms(const std::vector<Node*>& axioms);
    void clear_data();

    void find_formul(const Node& formula);
};

#endif