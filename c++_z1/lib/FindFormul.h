#ifndef FIND_FORMUL_H
#define FIND_FORMUL_H

#include "Rule.h"
#include "Formula.h"
#include <vector>
#include <unordered_map>

class FindFormul {
    std::vector<Rule*> rules;
    std::vector<Node*> axioms;
    std::vector<Node*> data;
    std::unordered_map<std::string, std::pair<std::string, std::string>> pathMap; 
    std::vector<Node*> deduction_axiom;

public:
    FindFormul();
    void set_rules(const std::vector<Rule*>& rules);
    void set_axioms(const std::vector<Node*>& axioms);
    void clear_data();
    
    std::string get_string_format(const Node& node);
    void recover_path(const std::string& find_node, std::ostream& out, int depth);
    void print_result(const Node& find_node);

    void find_formul(const Node& formula);
};

#endif