#include "Nullable.h"

void Nullable::visit(const RegexNode* node)
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

void Nullable::visit(const Intersection* node)
{
    stack.push_back(evaluate(node->left()) && evaluate(node->right()));
}

void Nullable::visit(const Complement* node)
{
    stack.push_back(!evaluate(node->expr()));
}

bool Nullable::evaluate(const RegexNode* regex)
{
    stack.clear();
    visit(regex);
    bool res = stack.back();
    stack.pop_back();
    return res;
}

bool Nullable::evaluate(const Regexp& regex)
{
    return evaluate(regex.get());
}

RegexNode* helper(const RegexNode* node)
{
    if (Nullable{}.evaluate(node))
        return new Epsilon;
    else
        return new Empty;
}

