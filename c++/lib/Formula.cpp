#include "Formula.h"
#include <iostream>
#include <stdexcept>

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


Node Formula::replace_chars(const Node& formula, char repl_char, const Node& replace_formula) {
    Node new_formula(formula);
    std::queue<Node*> q;
    q.push(&new_formula);

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        if (node->value == repl_char) {
            *node = replace_formula;
            continue;
        }

        if (node->left)
            q.push(node->left);
        if (node->right)
            q.push(node->right);
    }

    return new_formula;
}