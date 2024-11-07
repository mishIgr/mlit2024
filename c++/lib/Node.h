#ifndef NODE_H
#define NODE_H

#include <string>
#include <stack>

class Node {
public:
    char value;
    Node* left;
    Node* right;

    Node(char value = 0, Node* left = nullptr, Node* right = nullptr);
    Node(const Node& other);
    Node(Node&& other) noexcept;
    Node& operator=(const Node& other);
    Node& operator=(Node&& other) noexcept;
    ~Node();

    bool operator==(const Node& other) const;
    bool operator<(const Node& other) const;

    std::string to_string() const;
};

std::ostream& operator<<(std::ostream& out, const Node& node);

#endif
