#ifndef NODE_H
#define NODE_H

#include <string>
#include <stack>

#define CONST_VALUE -1
#define VARIBLE_VALUE 0

struct ValueNode {
    char symbol;
    int num_symbol;

    ValueNode(char symbol, int num_symbol);
    ValueNode(char symbol);
    ValueNode();

    bool operator==(char symbol) const;
    bool operator<(char symbol) const;
    bool operator==(const ValueNode& other) const;
    bool operator<(const ValueNode& other) const;

    bool is_varible_or_const() const;
};

std::ostream& operator<<(std::ostream& out, const ValueNode& node);


struct Node {
    ValueNode value;
    Node* left;
    Node* right;

    Node(ValueNode value, Node* left = nullptr, Node* right = nullptr);
    Node(char value = 0, Node* left = nullptr, Node* right = nullptr);
    Node(const Node& other, int num_symbol = 0);
    Node(Node&& other) noexcept;
    Node& operator=(const Node& other);
    Node& operator=(Node&& other) noexcept;
    ~Node();

    bool operator==(const Node& other) const;
    bool operator<(const Node& other) const;

    void set_all_const();
    void set_all_varible();

    bool equal(const Node& other) const;
};

std::ostream& operator<<(std::ostream& out, const Node& node);

#endif
