#ifndef RULES_H
#define RULES_H

#include "Formula.h"

struct Rules {
    virtual bool is_approp(const Node& first_formula, const Node& second_formula, Node& new_formula) const = 0;
};

struct ModusPonuns : public Rules {
    bool is_approp(const Node& first_formula, const Node& second_formula, Node& new_formula) const override;
};

#endif