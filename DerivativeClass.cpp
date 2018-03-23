//
// Created by devan on 3/23/18.
//

#include "DerivativeClass.h"

void DerivativeClass::visit(const RegexNode* node)
{
    node->accept(this);
}

void DerivativeClass::visit(const Closure* closure)
{

}

void DerivativeClass::visit(const Concat* concat)
{

}

void DerivativeClass::visit(const Union* anUnion)
{

}

void DerivativeClass::visit(const Intersection* intersection)
{

}

void DerivativeClass::visit(const Complement* complement)
{

}

void DerivativeClass::visit(const Symbol* symbol)
{

}

void DerivativeClass::visit(const Epsilon* epsilon)
{

}

void DerivativeClass::visit(const Empty* empty)
{

}

RegexNode* DerivativeClass::evaluate(const RegexNode* node)
{
    visit(node);
    RegexNode* res = stack.back();
    stack.pop_back();
    return res;
}
