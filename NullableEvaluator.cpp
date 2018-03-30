#include "NullableEvaluator.h"

void NullableEvaluator::visit(const RegexNode* node)
{
    node->accept(this);
}

void NullableEvaluator::visit(const Closure* node)
{
    stack.push_back(true);
}

void NullableEvaluator::visit(const Concat* node)
{
    stack.push_back(evaluate(node->left()) && evaluate(node->right()));
}

void NullableEvaluator::visit(const Union* node)
{
    stack.push_back(evaluate(node->left()) || evaluate(node->right()));
}

void NullableEvaluator::visit(const Symbol* node)
{
    stack.push_back(false);
}

void NullableEvaluator::visit(const Epsilon* node)
{
    stack.push_back(true);
}

void NullableEvaluator::visit(const Empty* node)
{
    stack.push_back(false);
}

void NullableEvaluator::visit(const Intersection* node)
{
    stack.push_back(evaluate(node->left()) && evaluate(node->right()));
}

void NullableEvaluator::visit(const Complement* node)
{
    stack.push_back(!evaluate(node->expr()));
}

bool NullableEvaluator::evaluate(const RegexNode* regex)
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

RegexNode* helper(const RegexNode* node)
{
    if (NullableEvaluator{}.evaluate(node))
        return new Epsilon;
    else
        return new Empty;
}

