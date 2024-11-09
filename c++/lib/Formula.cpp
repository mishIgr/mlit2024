#include "Formula.h"
#include <stdexcept>
#include <algorithm>

const char* ErrorReadFormula::what() const noexcept {
    return "Ошибка в чтении формулы";
}

Unifier::Unifier() {}
Unifier::Unifier(const Node& into, const Node& what) : into(into), what(what) {}

bool Unifier::equal_into(const Node& into) const { return this->into == into; }
bool Unifier::equal_what(const Node& what) const { return this->what == what; }
Node Unifier::get_into() const { return Node(into); }
Node Unifier::get_what() const { return Node(what); }

std::ostream& operator<<(std::ostream& out, const Unifier& other) {
    out << '{' << other.what << " | " << other.into << '}';
    return out;
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

void Formula::replace_chars(Node& formula, const std::vector<Unifier>& data) {
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
