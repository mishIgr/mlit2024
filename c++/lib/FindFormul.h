#ifndef FIND_FORMUL_H
#define FIND_FORMUL_H

#include "Rules.h"
#include "Formula.h"
#include <vector>

class FindFormul {
    std::vector<Rules*> rules;
    std::vector<Node*> axioms;
    std::vector<Node*> simple_substitutions;
    std::string permis_chars;
    std::vector<Node*> data;

public:
    FindFormul();
    void set_rules(const std::vector<Rules*>& rules);
    void set_axioms(const std::vector<Node*>& axioms);
    void set_simple_substitutions(const std::vector<Node*>& simpel_substitutions);
    void set_permis_chars(const std::string permis_chars);
    void clear_data();

    void find_formul(const Node& formula);
};

#endif