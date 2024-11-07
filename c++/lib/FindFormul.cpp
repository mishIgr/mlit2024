#include "FindFormul.h"
#include <iostream>
#include <algorithm>

std::ostream& operator<<(std::ostream& out, const std::vector<Node*>& v) {
    for (Node* f : v)
        std::cout << *f << '\n';
    return out;
}

FindFormul::FindFormul() {}

void FindFormul::set_rules(const std::vector<Rules*>& rules) {
    this->rules = std::vector<Rules*>(rules.size(), nullptr);
    std::copy(rules.begin(), rules.end(), this->rules.begin());
}

void FindFormul::set_axioms(const std::vector<Node*>& axioms) {
    this->axioms = std::vector<Node*>(axioms.size(), nullptr);
    std::copy(axioms.begin(), axioms.end(), this->axioms.begin());
}

void FindFormul::set_simple_substitutions(const std::vector<Node*>& simple_substitutions) {
    this->simple_substitutions = std::vector<Node*>(simple_substitutions.size(), nullptr);
    std::copy(simple_substitutions.begin(), simple_substitutions.end(), this->simple_substitutions.begin());
}

void FindFormul::set_permis_chars(const std::string permis_chars) { this->permis_chars = permis_chars; }

void FindFormul::clear_data() {
    for (Node* ptr : data)
        delete ptr;
}

void FindFormul::find_formul(const Node& formula) {
    bool flag_find = false;
    size_t size_data;
    Node tmp_data;
    Node new_data;

    for (auto axiom : axioms)
        data.push_back(new Node(*axiom));

    while (!flag_find || data.size() < 1e10) {
        size_data = data.size();
        for (int i = 0; i < size_data; ++i) {
            for (int j = 0; j < i; ++j) {
                for (auto simp_sub : simple_substitutions) {
                    for (auto permis_char : permis_chars) {
                        for (Rules* rule : rules) {
                            tmp_data = Formula::replace_chars(*data[i], permis_char, *simp_sub);
                            if (rule->is_approp(tmp_data, *data[j], new_data)) {

                                auto it = std::find_if(data.begin(), data.end(), [new_data](Node* node) {
                                    return node && *node == new_data; 
                                });

                                if (it != data.end())
                                    continue;

                                if (new_data == formula) {
                                    std::cout << "Формула найдена на количестве элементов " << data.size() << '\n';
                                    return ;
                                }
                                data.push_back(new Node(new_data));
                            }
                            // else {
                            //     data.push_back(new Node(tmp_data));
                            // }
                        }
                    }
                }
            }
        }
        std::cout << "Текущий размер данных " << data.size() << '\n';
        // if (data.size() > 500) std::cout << data;
    }
}