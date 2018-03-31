#include "NullableEvaluator.h"

void NullableEvaluator::visit(const Regex::Node* node)
{
    node->accept(this);
}

void NullableEvaluator::visit(const Regex::Closure* node)
{
    stack.push_back(true);
}

void NullableEvaluator::visit(const Regex::Concat* node)
{
    stack.push_back(evaluate(node->left()) && evaluate(node->right()));
}

void NullableEvaluator::visit(const Regex::Union* node)
{
    stack.push_back(evaluate(node->left()) || evaluate(node->right()));
}

void NullableEvaluator::visit(const Regex::Symbol* node)
{
    stack.push_back(false);
}

void NullableEvaluator::visit(const Regex::Epsilon* node)
{
    stack.push_back(true);
}

void NullableEvaluator::visit(const Regex::Empty* node)
{
    stack.push_back(false);
}

void NullableEvaluator::visit(const Regex::Intersection* node)
{
    stack.push_back(evaluate(node->left()) && evaluate(node->right()));
}

void NullableEvaluator::visit(const Regex::Complement* node)
{
    stack.push_back(!evaluate(node->expr()));
}

bool NullableEvaluator::evaluate(const Regex::Node* regex)
{
    stack.clear();
    visit(regex);
    bool res = stack.back();
    stack.pop_back();
    return res;
}

bool NullableEvaluator::evaluate(const Regexp& regex)
{
    return evaluate(regex.get());
}

Regex::Node* helper(const Regex::Node* node)
{
    if (NullableEvaluator{}.evaluate(node))
        return new Regex::Epsilon;
    else
        return new Regex::Empty;
}

