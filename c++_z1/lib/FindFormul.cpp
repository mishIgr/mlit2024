#include "FindFormul.h"
#include <iostream>
#include <algorithm>

std::ostream& operator<<(std::ostream& out, const std::vector<Node*>& v) {
    for (Node* f : v)
        std::cout << *f << '\n';
    return out;
}

FindFormul::FindFormul() {}

void FindFormul::set_rules(const std::vector<Rule*>& rules) {
    this->rules = std::vector<Rule*>(rules.size(), nullptr);
    std::copy(rules.begin(), rules.end(), this->rules.begin());
}

void FindFormul::set_axioms(const std::vector<Node*>& axioms) {
    this->axioms = std::vector<Node*>(axioms.size(), nullptr);
    std::copy(axioms.begin(), axioms.end(), this->axioms.begin());
}

void FindFormul::clear_data() {
    for (Node* ptr : data)
        delete ptr;
}

template <typename T>
void printDeduction(const T& data, const Node& right_value) {
    std::cout << "Дедукция: ";
    for (auto& f : data)
        std::cout << *f << ", ";
    std::cout << " |- " << right_value << std::endl;
}


void FindFormul::recover_path(Node* find_node, std::ostream& out, int depth = 0) {
    Node* left = pathMap[find_node].first;
    Node* right = pathMap[find_node].second;
    out << '|' << std::string(depth * 2, '-') << *find_node << '\n';

    if (left != nullptr)
        recover_path(left, out, depth + 1);
    if (right != nullptr)
        recover_path(right, out, depth + 1);
}

void FindFormul::print_result(Node* find_node) {
    for (auto axiom : axioms)
        std::cout << "Axiom: " << *axiom << '\n';
    for (auto axiom : deduction_axiom)
        std::cout << "Deduction axiom: " << *axiom << '\n';
    recover_path(find_node, std::cout, 0);
}

void FindFormul::find_formul(const Node& formula) {
    bool flag_find = false;
    size_t size_data;
    Node new_data;
    std::string path_to_formul;

    for (auto axiom : axioms)
        data.push_back(new Node(*axiom));

    Node tmp_formula(formula);
    Node* p_tmp_formula = &tmp_formula;
    while (p_tmp_formula->value == '>') {
        auto it = std::find_if(data.begin(), data.end(), [p_tmp_formula](Node* node) {
            return *(p_tmp_formula->left) == *node; 
        });
        if (it == data.end()) {
            data.push_back(new Node(*p_tmp_formula->left));
            deduction_axiom.push_back(new Node(*p_tmp_formula->left));
        }
        p_tmp_formula = p_tmp_formula->right;
        printDeduction(data, *p_tmp_formula);
    }

    auto it = std::find_if(data.begin(), data.end(), [p_tmp_formula](Node* node) {
        return *(p_tmp_formula) == *node; 
    });
    if (it != data.end()) {
        std::cout << "Найденно по дедукции" << std::endl;
        return ;
    }

    for (Node* f : data) {
        pathMap[f] = std::make_pair(nullptr, nullptr);
    }

    while (true) {
        size_data = data.size();
        for (int i = 0; i < size_data; ++i) {
            for (int j = 0; j < size_data; ++j) {
                for (Rule* rule : rules) {
                    if (rule->is_approp({*data[i], i + 1}, {*data[j], i == j ? i + 2 : j + 1}, new_data)) {
                        auto it = std::find_if(data.begin(), data.end(), [new_data](Node* node) {
                            return node->equal(new_data); 
                        });
                        Node* temp = new Node(new_data);
                        if (it != data.end())
                            continue;
                        data.push_back(temp);

                        std::cout << rule->get_name() << '(' << i << ", " << j << "): ";
                        std::cout << *data[i] << ' ' << *data[j] << std::endl;
                        
                        pathMap[temp] = std::make_pair(data[i], data[j]);

                        if (*p_tmp_formula == new_data) {
                            std::cout << "Формула найдена на количестве элементов " << data.size() << '\n';
                            std::cout << "Найденная формула: " << new_data << "\tИкомая формула: " << *p_tmp_formula << '\n';
                            print_result(temp);
                            return ;
                        }
                    }
                }
            }
        }
    }
}
