#include "Nullable.h"

void Nullable::visit(const Regex* node)
{
    node->accept(this);
}

void Nullable::visit(const Closure* node)
{
    stack.push_back(true);
}

void Nullable::visit(const Concat* node)
{
    stack.push_back(evaluate(node->left()) && evaluate(node->right()));
}

void Nullable::visit(const Union* node)
{
    stack.push_back(evaluate(node->left()) || evaluate(node->right()));
}

void Nullable::visit(const Symbol* node)
{
    stack.push_back(false);
}

void Nullable::visit(const Epsilon* node)
{
    stack.push_back(true);
}

void Nullable::visit(const Empty* node)
{
    stack.push_back(false);
}

bool Nullable::evaluate(const Regex* node)
{
    visit(node);
    bool res = stack.back();
    stack.pop_back();
    return res;
}

void Nullable::visit(const Intersection* node)
{
    stack.push_back(evaluate(node->left()) && evaluate(node->right()));
}

void Nullable::visit(const Complement* node)
{
    stack.push_back(!evaluate(node->expr()));
}

Regex* helper(const Regex* node)
{
    if (Nullable{}.evaluate(node))
        return new Epsilon;
    else
        return new Empty;
}
