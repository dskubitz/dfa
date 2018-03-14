#include <iostream>
#include "DFAFunctionCalculator.h"

DFAFunctionCalculator::DFAFunctionCalculator(ASTNode& re)
        :tree_(re)
{
    followpos_.resize(CharNode::max_id() - 1);
    for (int i = 1; i < CharNode::max_id(); ++i) {
        followpos_[i - 1] = make_bitset();
    }
    tree_.accept(*this);

    /*
    std::vector<int> remaining;
    for (auto& pair : followpos_) {
        for (int i : pair.second) {
            auto it = followpos_.find(i);
            if (it == followpos_.end()) {
                remaining.push_back(i);
            }
        }
    }
    for (int i : remaining) {
        followpos_[i] = {};
    }
    */
}

void DFAFunctionCalculator::visit(StarNode& node)
{
    ASTNode* expr = node.expr();

    expr->accept(*this);

    nullable_[&node] = true;
    auto& firstpos = firstpos_[&node];
    auto& lastpos = lastpos_[&node];

    firstpos = lastpos = firstpos_.at(expr);

//    firstpos.insert(firstpos_.at(expr).begin(), firstpos_.at(expr).end());

//    lastpos.insert(firstpos.begin(), firstpos.end());

    for (int i = 1; i < CharNode::max_id(); ++i)
        if (lastpos.test(i))
            followpos_[i - 1] |= firstpos;

//    for (int i : lastpos)
//        followpos_[i].insert(firstpos.begin(), firstpos.end());
}

void DFAFunctionCalculator::visit(CatNode& node)
{
    ASTNode* left = node.left();
    ASTNode* right = node.right();

    left->accept(*this);
    right->accept(*this);

    nullable_[&node] = nullable_.at(left) && nullable_.at(right);
    auto& firstpos = firstpos_[&node];
    auto& lastpos = lastpos_[&node];

    firstpos = firstpos_.at(left);
    if (nullable_.at(left))
        firstpos |= firstpos_.at(right);

    lastpos = firstpos_.at(right);
    if (nullable_.at(right))
        lastpos |= firstpos_.at(left);

    auto& lastpos_c1 = lastpos_.at(left);
    auto& firstpos_c2 = firstpos_.at(right);

    for (int i = 1; i < CharNode::max_id(); ++i)
        if (lastpos_c1.test(i))
            followpos_[i - 1] |= firstpos_c2;
}

void DFAFunctionCalculator::visit(UnionNode& node)
{
    ASTNode* left = node.left();
    ASTNode* right = node.right();
    left->accept(*this);
    right->accept(*this);

    nullable_[&node] = nullable_.at(left) || nullable_.at(right);

    lastpos_[&node] = firstpos_[&node] = firstpos_.at(left) | firstpos_.at(right);
}

void DFAFunctionCalculator::visit(CharNode& node)
{
    nullable_[&node] = false;
    firstpos_[&node] = make_bitset(node.id());
    lastpos_[&node] = make_bitset(node.id());
}

void DFAFunctionCalculator::visit(EpsilonNode& node)
{
    nullable_[&node] = true;
    firstpos_[&node] = make_bitset();
    lastpos_[&node] = make_bitset();
}

void DFAFunctionCalculator::visit(EndmarkerNode& node)
{
}
