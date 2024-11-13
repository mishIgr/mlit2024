#include "Formula.h"
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <stack>
#include <queue>

const char* ErrorReadFormula::what() const noexcept {
    return "Ошибка в чтении формулы";
}

// Приоритет операции
std::unordered_map<char, int> precedence = {
    {'!', 5},  // Отрицание
    {'*', 4},  // Конъюнкция
    {'|', 3},  // Дизъюнкция
    {'>', 2},  // Имплекация
    {'+', 2},  // XOR
    {'=', 1}   // Эквиваленция
};

Node Formula::to_expression_tree(const std::string& expression) {
    std::stack<Node*> stack;
    std::stack<char> operator_stack;

    for (size_t i = 0; i < expression.length(); ++i) {
        char token = expression[i];

        if (std::isspace(token))
            continue;
        else if (std::isalnum(token))
            stack.push(new Node(token));
        else if (precedence.count(token)) {
            while (!operator_stack.empty() && precedence.count(operator_stack.top()) &&
                   precedence[operator_stack.top()] >= precedence[token]) {
                char op = operator_stack.top();
                operator_stack.pop();

                if (op == '!') {
                    Node* operand = stack.top();
                    stack.pop();
                    stack.push(new Node(op, operand));
                } else {
                    Node* right = stack.top();
                    stack.pop();
                    Node* left = stack.top();
                    stack.pop();
                    stack.push(new Node(op, left, right));
                }
            }
            operator_stack.push(token);
        } else if (token == '(') {
            operator_stack.push(token);
        } else if (token == ')') {
            while (!operator_stack.empty() && operator_stack.top() != '(') {
                char op = operator_stack.top();
                operator_stack.pop();

                if (op == '!') {
                    Node* operand = stack.top();
                    stack.pop();
                    stack.push(new Node(op, operand));
                } else {
                    Node* right = stack.top();
                    stack.pop();
                    Node* left = stack.top();
                    stack.pop();
                    stack.push(new Node(op, left, right));
                }
            }
            operator_stack.pop();
        } else {
            std::cerr << "Unknown character: " << token << '\n';
            throw ErrorReadFormula();
        }
    }

    while (!operator_stack.empty()) {
        char op = operator_stack.top();
        operator_stack.pop();

        if (op == '!') {
            Node* operand = stack.top();
            stack.pop();
            stack.push(new Node(op, operand));
        } else {
            Node* right = stack.top();
            stack.pop();
            Node* left = stack.top();
            stack.pop();
            stack.push(new Node(op, left, right));
        }
    }

    return *stack.top();
}

std::vector<Unifier> Formula::replace_chars(Node& formula, const ValueNode& repl_value, const Node& replace_formula) {
    std::vector<Unifier> data;
    std::queue<Node*> q;
    q.push(&formula);

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        if (node->value == repl_value) {
            auto it = std::find_if(data.begin(), data.end(), [node](const Unifier& unifier) {
                return unifier.equal_into(*node); 
            });
            if (it == data.end())
                data.push_back(Unifier(*node, replace_formula));
            *node = replace_formula;
            continue;
        }

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
    return data;
}

std::vector<Unifier> Formula::replace_chars(Node& formula, const std::vector<Unifier>& data) {
    std::queue<Node*> q;
    q.push(&formula);

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        if (node->value.num_symbol != 0) {
            auto it = std::find_if(data.begin(), data.end(), [node](const Unifier& unifier) {
                return unifier.equal_into(*node); 
            });
            if (it != data.end()) {
                *node = it->get_what();
                continue;
            }
        }

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
    return data;
}

std::vector<Unifier> Formula::to_zerros_num_value(Node& formula) {
    std::vector<Unifier> data;
    std::queue<Node*> q;
    q.push(&formula);

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        if (node->value.num_symbol != 0) {
            auto it = std::find_if(data.begin(), data.end(), [node](const Unifier& unifier) {
                return unifier.equal_into(*node); 
            });
            if (it == data.end()) {
                Node tmp(*node);
                node->value.num_symbol = 0;
                data.push_back(Unifier(tmp, *node));
            }
            else node->value.num_symbol = 0;
        }

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
    return data;
}

std::pair<Node*, Node*> Formula::get_first_mismatched_pair(Node& first_formula, Node& second_formula) {
    std::stack<Node*> first;
    first.push(&first_formula);
    std::stack<Node*> second;
    second.push(&second_formula);
    auto is_end_node = [](Node* node){ return node->value.num_symbol != 0 && !node->left && !node->right; };

    while (!first.empty() && !second.empty()) {
        auto node_first = first.top();
        first.pop();
        auto node_second = second.top();
        second.pop();

        if (node_first->value != node_second->value)
            return {node_first, node_second};

        if (node_first->left) first.push(node_first->left);
        if (node_first->right) first.push(node_first->right);
        if (node_second->left) second.push(node_second->left);
        if (node_second->right) second.push(node_second->right);

    }

    return {nullptr, nullptr};
}

std::vector<Unifier> Formula::unification(Node& first, Node& left) {
    std::vector<Unifier> data;
    Node *first_formula, *left_formula;
    std::tie(first_formula, left_formula) = Formula::get_first_mismatched_pair(first, left);

    while (first_formula && left_formula) {
        if (!left_formula->left && !left_formula->right) {
            auto un = Formula::to_zerros_num_value(*first_formula);

            if (un.size() == 0)
                return {};

            data.insert(data.end(), un.begin(), un.end());
            Formula::replace_chars(first, un);
            un = Formula::replace_chars(left, { Unifier(*left_formula, *first_formula) });
            data.insert(data.end(), un.begin(), un.end());
        }
        else if (!first_formula->left && !first_formula->right) {
            auto un = Formula::to_zerros_num_value(*left_formula);

            if (un.size() == 0)
                return {};

            data.insert(data.end(), un.begin(), un.end());
            Formula::replace_chars(left, un);
            un = Formula::replace_chars(first, { Unifier(*first_formula, *left_formula) });
            data.insert(data.end(), un.begin(), un.end());
        }
        else return {};

        std::tie(first_formula, left_formula) = Formula::get_first_mismatched_pair(first, left);
    }
    return data;
}
