#include <iostream>
#include "DFAFunctionCalculator.h"

DFAFunctionCalculator::DFAFunctionCalculator(ASTNode* re)
        :tree_(re)
{
    followpos_.resize(CharNode::max_id());
    symbols_.resize(CharNode::max_id());
    for (int i = 0; i < CharNode::max_id(); ++i)
        followpos_[i] = make_bitset();

    tree_->accept(*this);
}

void DFAFunctionCalculator::visit(StarNode& node)
{
    ASTNode* expr = node.expr();

    expr->accept(*this);

    nullable_[&node] = true;
    auto& firstpos = firstpos_[&node];
    auto& lastpos = lastpos_[&node];

    firstpos = lastpos = firstpos_.at(expr);

    for (size_t i = 0; i < CharNode::max_id(); ++i)
        if (lastpos.test(i))
            followpos_[i] |= firstpos;
}

void DFAFunctionCalculator::visit(CatNode& node)
{
    ASTNode* left = node.left();
    ASTNode* right = node.right();

    left->accept(*this);
    right->accept(*this);

    nullable_[&node] = nullable_.at(left) && nullable_.at(right);

    firstpos_[&node] = firstpos_.at(left);
    if (nullable_.at(left))
        firstpos_[&node] |= firstpos_.at(right);

    lastpos_[&node] = firstpos_.at(right);
    if (nullable_.at(right))
        lastpos_[&node] |= firstpos_.at(left);

    auto& lastpos_c1 = lastpos_.at(left);
    auto& firstpos_c2 = firstpos_.at(right);

    for (size_t i = 0; i < CharNode::max_id(); ++i)
        if (lastpos_c1.test(i))
            followpos_[i] |= firstpos_c2;
}

void DFAFunctionCalculator::visit(UnionNode& node)
{
    ASTNode* left = node.left();
    ASTNode* right = node.right();
    left->accept(*this);
    right->accept(*this);

    nullable_[&node] = nullable_.at(left) || nullable_.at(right);
    firstpos_[&node] = firstpos_.at(left) | firstpos_.at(right);
    lastpos_[&node] = firstpos_.at(left) | firstpos_.at(right);
}

void DFAFunctionCalculator::visit(CharNode& node)
{
    nullable_[&node] = false;
    firstpos_[&node] = make_bitset(node.id());
    lastpos_[&node] = make_bitset(node.id());
    symbols_[node.id()] = node.value();
}

void DFAFunctionCalculator::visit(EpsilonNode& node)
{
    nullable_[&node] = true;
    firstpos_[&node] = make_bitset();
    lastpos_[&node] = make_bitset();
}

void DFAFunctionCalculator::visit(EndmarkerNode& node)
{
    nullable_[&node] = false;
    firstpos_[&node] = make_bitset(node.id());
    lastpos_[&node] = make_bitset(node.id());
    symbols_[node.id()] = node.value();
    acceptpos_[node.id()] = node.name();
    std::cout << node.id() << ' ' << node.name() << '\n';
}


