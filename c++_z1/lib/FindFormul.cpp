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

Node new_axiom(std::vector<Node*> axioms, std::unordered_map<std::string, std::pair<std::string, std::string>> pathMap, Rule* rule) {
    std::vector<std::pair<int, int>> indexs {
        {0, 0},
        {1, 0},
        {3, 1},
        {4, 1},
        {2, 5},
        {6, 6},
        {7, 8},
        {3, 9}
    };
    Node tmp;
    for (auto [i, j] : indexs) {
        bool f = rule->is_approp({*axioms[i], 1}, {*axioms[j], 2}, tmp);
        if (!f)
            std::cout << "OSHIBKA\n";
        axioms.push_back(new Node(tmp));
    }
    for (auto p : axioms)
        std::cout << *p << std::endl;
    return tmp;
}


void FindFormul::find_formul(const Node& formula) {
    bool flag_find = false;
    size_t size_data;
    Node new_data;
    std::string path_to_formul;

    for (auto axiom : axioms)
        data.push_back(new Node(*axiom));
    
    // new_axiom(data, pathMap, rules[0]);
    
    Node tmp_formula(formula);
    Node* p_tmp_formula = &tmp_formula;
    while (p_tmp_formula->value == '>') {
        data.push_back(new Node(*p_tmp_formula->left));
        deduction_axiom.push_back(new Node(*p_tmp_formula->left));
        p_tmp_formula = p_tmp_formula->right;
        printDeduction(data, *p_tmp_formula);
    }
    // for (auto& f : data)
    //     std::cout << *f << ", ";
    // std::cout << "AAAAA "<<std::endl;

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
                        
                        pathMap[temp] = std::make_pair(data[i], data[j]);

                        std::cout << "md(" << i << ", " << j << ") ";
                        std::cout << *data[i] << ' ' << *data[j] << " |- " << new_data << std::endl;
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
