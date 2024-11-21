#ifndef RULE_H
#define RULE_H

#include "Formula.h"

struct Rule {
    virtual bool is_approp(const std::pair<const Node&, int>& first_formula, const std::pair<const Node&, int>& second_formula, Node& new_formula) const = 0;
    virtual std::string get_name() const = 0;
};

#endif