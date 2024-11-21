#ifndef UNIFIER_H
#define UNIFIER_H

#include "Node.h"

class ErrorCreateUnifier : public std::exception {
public:
    const char* what() const noexcept override;
};

class Unifier {
    Node into;
    Node what;

public:
    Unifier();
    Unifier(const Node& into, const Node& what);

    bool equal_into(const Node& into) const;
    bool equal_what(const Node& what) const;
    Node get_into() const;
    Node get_what() const;
    ValueNode get_value_into() const;
    ValueNode get_value_what() const;

    friend std::ostream& operator<<(std::ostream& out, const Unifier& other);
};

#endif