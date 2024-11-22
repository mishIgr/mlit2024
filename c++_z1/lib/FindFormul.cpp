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

std::string FindFormul::getStringFormat(const Node& node) {
    std::string data;
    if (node.right)
        data += '(' + getStringFormat(*node.left) + ' ' + node.value.symbol  + ' ' + getStringFormat(*node.right) + ')';
    else if (node.left)
        data += node.value.symbol + getStringFormat(*node.left);
    else 
        data += node.value.symbol;
    return data;
}

void FindFormul::recoverPath(const std::string& find_node, std::ostream& out, int depth = 0) {
    auto [left, right] = pathMap[find_node];
    out << '|' << std::string(depth * 2, '-') << find_node << '\n';
    if (left != "")
        recoverPath(left, out, depth + 1);
    if (right != "")
        recoverPath(right, out, depth + 1);
}

void FindFormul::printResult(const Node& find_node) {
    for (auto axiom : axioms)
        std::cout << "Axiom: " << *axiom << '\n';
    for (auto axiom : deduction_axiom)
        std::cout << "Deduction axiom: " << *axiom << '\n';
    recoverPath(getStringFormat(find_node), std::cout, 0);
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
        data.push_back(new Node(*p_tmp_formula->left));
        deduction_axiom.push_back(new Node(*p_tmp_formula->left));
        p_tmp_formula = p_tmp_formula->right;
        printDeduction(data, *p_tmp_formula);
    }
    // for (auto& f : data)
    //     std::cout << *f << ", ";
    // std::cout << "AAAAA "<<std::endl;

    for (auto& f : data) {
        pathMap[getStringFormat(*f)] = std::make_pair("", "");
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

                        if (it != data.end())
                            continue;
                        data.push_back(new Node(new_data));

                        pathMap[getStringFormat(new_data)] = std::make_pair(getStringFormat(*data[i]), getStringFormat(*data[j]));

                        std::cout << "md(" << i << ", " << j << ") ";
                        std::cout << *data[i] << ' ' << *data[j] << " |- " << new_data << std::endl;
                        if (p_tmp_formula->equal(new_data)) {
                            std::cout << "Формула найдена на количестве элементов " << data.size() << '\n';
                            std::cout << "Найденная формула: " << new_data << "\tИкомая формула: " << *p_tmp_formula << '\n';
                            printResult(*p_tmp_formula);
                            return ;
                        }
                    }
                }
            }
        }
        std::cout << "Размер данных " << data.size() << '\n';
    }
}
