#include "Formula.h"
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <stack>
#include <queue>

const char* ErrorReadFormula::what() const noexcept {
    return "Ошибка в чтении формулы";
}

// Выдаёт элемент по ключу, если ключа нет, то добавляет новую запись (key, next_value), и перемещает next_value на следующий символ
ValueNode ReplaceParam::get_or_default(std::map<ValueNode, ValueNode>& data, const ValueNode& key) {
    auto it = data.find(key);
    if (it != data.end())
        return it->second;

    data[key] = next_value;
    return next_value++;
}

// Статический метод для определения следующего символа, который ещё не используется в формуле
char ReplaceParam::find_next_value(Node* formula) {
    char find_char = FIRST_VALUE;
    std::queue<Node*> q;
    q.push(formula);

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        if (node->value.num_symbol == VARIBLE_VALUE && !node->left && !node->right)
            find_char = std::max(find_char, node->value.symbol);

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }

    return ++find_char;
}

ReplaceParam::ReplaceParam(char next_value) : next_value(next_value) {}

// Зануляет все переменные, которые имеют значение num_symbol != (0 | -1). Замены добавляет в replace_data, unif_data
void ReplaceParam::to_zerros_num_value(Node* formula, std::map<ValueNode, ValueNode>& replace_data, std::vector<Unifier>& unif_data) {
    std::queue<Node*> q;
    q.push(formula);

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        if (!node->value.is_varible_or_const()) {
            auto new_value_node = get_or_default(replace_data, node->value);

            auto it = std::find_if(unif_data.begin(), unif_data.end(), [node, new_value_node](const Unifier& unif){
                return unif.get_value_into() == node->value && unif.get_value_what() == new_value_node;
            });
            if (it == unif_data.end())
                unif_data.push_back(Unifier(*node, Node(new_value_node)));
            
            node->value = new_value_node;
        }

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}

// Меняет все переменные, которые имеют значение num_symbol != 0, данные берутся из unif_data.
void ReplaceParam::change_form_to_var(Node* formula, const std::vector<Unifier>& unif_data) {
    bool flag_end_change = false;
    while (!flag_end_change) {
        flag_end_change = true;
        std::queue<Node*> q;
        q.push(formula);

        while (!q.empty()) {
            Node* node = q.front();
            q.pop();

            if (!node->value.is_varible_or_const() || node->value.num_symbol == VARIBLE_VALUE && !node->left && !node->right) {
                auto it = std::find_if(unif_data.begin(), unif_data.end(), [node](const Unifier& unif){
                    return unif.get_value_into() == node->value;
                });
                if (it != unif_data.end()) {
                    *node = it->get_what();
                    flag_end_change = false;
                }
                continue;
            }

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
}

// Удаляет два подряд идущих отрицаний отрицания
void ReplaceParam::del_repet_denial(Node* formula) {
    std::queue<Node*> q;
    q.push(formula);

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        if (node->value == '!' && node->left->value == '!') {
            Node* tmp = node->left->left;
            *node = *tmp;
        }

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
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

// Функция парсит строку и переводит её в Node
Node Formula::to_expression_tree(const std::string& expression) {
    if (expression.empty())
        throw std::logic_error("Ожидается не пустая строка");
        
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

// Функця проверяет, можно ли унифицировать два высказывания
bool Formula::unification(std::pair<const Node&, int> first_formula, std::pair<const Node&, int> second_formula, std::vector<Unifier>& unif_data) {
    std::map<ValueNode, ValueNode> replace_data;

    ReplaceParam replace_param;
    
    Node unif_first_formula(first_formula.first, first_formula.second);
    Node unif_second_formula(second_formula.first, second_formula.second);

    std::stack<Node*> first;
    std::stack<Node*> second;
    first.push(&unif_first_formula);
    second.push(&unif_second_formula);

    int c = 0;

    while (!first.empty() && !second.empty()) {
        auto first_node = first.top();
        first.pop();

        auto second_node = second.top();
        second.pop();

        if (first_node->value == second_node->value) {
            if (first_node->right) first.push(first_node->right);
            if (first_node->left) first.push(first_node->left);
            if (second_node->right) second.push(second_node->right);
            if (second_node->left) second.push(second_node->left);
        }

        else if (!first_node->value.is_varible_or_const() || !second_node->value.is_varible_or_const()) {
            Node* value_node = !first_node->value.is_varible_or_const() ? first_node : second_node;
            Node* other_node = first_node->value.is_varible_or_const() ? first_node : second_node;
            Node* value_formula = !first_node->value.is_varible_or_const() ? &unif_first_formula : &unif_second_formula;
            Node* other_formula = first_node->value.is_varible_or_const() ? &unif_first_formula : &unif_second_formula;

            replace_param.to_zerros_num_value(other_node, replace_data, unif_data);
            replace_param.change_form_to_var(other_formula, unif_data);

            unif_data.push_back(Unifier(*value_node, *other_node));
            replace_param.change_form_to_var(value_formula, unif_data);
        }

        else if (first_node->value.num_symbol == VARIBLE_VALUE && second_node->value.num_symbol == CONST_VALUE) {
            if (first_node->left || first_node->right)
                return false;

            unif_data.push_back(Unifier(*second_node, *first_node));
            replace_param.change_form_to_var(&unif_first_formula, unif_data);
        }

        else if (first_node->value.num_symbol == CONST_VALUE && second_node->value.num_symbol == VARIBLE_VALUE) {
            if (second_node->left || second_node->right)
                return false;

            unif_data.push_back(Unifier(*first_node, *second_node));
            replace_param.change_form_to_var(&unif_second_formula, unif_data);
        }

        else if (first_node->value == '!' && !first_node->left->left && !first_node->left->right) {
            replace_param.to_zerros_num_value(second_node, replace_data, unif_data);
            replace_param.change_form_to_var(&unif_second_formula, unif_data);

            unif_data.push_back(Unifier(*first_node->left, Node('!', new Node(*second_node))));
            replace_param.change_form_to_var(&unif_first_formula, unif_data);
        }

        else if (second_node->value == '!' && !second_node->left->left && !second_node->left->right) {
            replace_param.to_zerros_num_value(first_node, replace_data, unif_data);
            replace_param.change_form_to_var(&unif_first_formula, unif_data);

            unif_data.push_back(Unifier(*second_node->left, Node('!', new Node(*first_node))));
            replace_param.change_form_to_var(&unif_second_formula, unif_data);
        }

        else return false;
    }

    return first.empty() && second.empty();
}
