#include "ModusPonuns.h"
#include <iostream>
#include <string>

bool ModusPonuns::is_approp(const std::pair<const Node&, int>& first_formula, const std::pair<const Node&, int>& second_formula, Node& new_formula) const {
    if (second_formula.first.value != '>')
        return false;
    
    std::vector<Unifier> unif_data;

    try {
        if (!Formula::unification(first_formula, {*second_formula.first.left, second_formula.second}, unif_data))
            return false;
        
        ReplaceParam rp;
        std::map<ValueNode, ValueNode> replace_data;
        new_formula = Node(*second_formula.first.right, second_formula.second);
        rp.change_form_to_var(&new_formula, unif_data);
        rp.del_repet_denial(&new_formula);

        // for (auto& u : unif_data)
        //     std::cout << u << ' ';
        // std::cout << std::endl;

        rp = ReplaceParam(ReplaceParam::find_next_value(&new_formula));
        rp.to_zerros_num_value(&new_formula, replace_data, unif_data);

        // for (auto& u : unif_data)
        //     std::cout << u << ' ';
        // std::cout << std::endl;

        return true;
    } catch (ErrorCreateUnifier) {
        return false;
    } catch (const std::exception& e) {
        throw ;
    }
}

std::string ModusPonuns::get_name() const { return "mp"; }
