#ifndef MODUSPONUNS_H
#define MODUSPONUNS_H

#include "Rule.h"

struct ModusPonuns : public Rule {
    bool is_approp(const std::pair<const Node&, int>& first_formula, const std::pair<const Node&, int>& second_formula, Node& new_formula) const override;
    std::string get_name() const override;
};

#endif
