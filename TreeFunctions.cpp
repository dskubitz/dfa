#include <iostream>
#include "TreeFunctions.h"
void TreeFunctions::visit(const ASTNode* node)
{
    node->accept(this);
}

TreeFunctions::TreeFunctions(const ASTNode* re)
        : tree_(re)
{
    followpos_.resize(CharNode::max_id());
    symbols_.resize(CharNode::max_id());

    for (int i = 0; i < CharNode::max_id(); ++i)
        followpos_[i] = make_bitset();

    visit(tree_);
}

void TreeFunctions::visit(const StarNode* node)
{
    const ASTNode* expr = node->expr();

    visit(expr);

    nullable_[node] = true;
    auto& firstpos = firstpos_[node];
    auto& lastpos = lastpos_[node];

    firstpos = lastpos = firstpos_.at(expr);

    for (size_t i = 0; i < CharNode::max_id(); ++i)
        if (lastpos.test(i))
            followpos_[i] |= firstpos;
}

void TreeFunctions::visit(const CatNode* node)
{
    const ASTNode* left = node->left();
    const ASTNode* right = node->right();

    visit(left);
    visit(right);

    nullable_[node] = nullable_.at(left) && nullable_.at(right);

    firstpos_[node] = firstpos_.at(left);
    if (nullable_.at(left))
        firstpos_[node] |= firstpos_.at(right);

    lastpos_[node] = firstpos_.at(right);
    if (nullable_.at(right))
        lastpos_[node] |= firstpos_.at(left);

    auto& lastpos_c1 = lastpos_.at(left);
    auto& firstpos_c2 = firstpos_.at(right);

    for (size_t i = 0; i < CharNode::max_id(); ++i)
        if (lastpos_c1.test(i))
            followpos_[i] |= firstpos_c2;
}

void TreeFunctions::visit(const UnionNode* node)
{
    const ASTNode* left = node->left();
    const ASTNode* right = node->right();

    visit(left);
    visit(right);

    nullable_[node] = nullable_.at(left) || nullable_.at(right);
    firstpos_[node] = firstpos_.at(left) | firstpos_.at(right);
    lastpos_[node] = firstpos_.at(left) | firstpos_.at(right);
}

void TreeFunctions::visit(const CharNode* node)
{
    nullable_[node] = false;
    firstpos_[node] = make_bitset(node->id());
    lastpos_[node] = make_bitset(node->id());
    symbols_[node->id()] = node->value();
}

void TreeFunctions::visit(const EpsilonNode* node)
{
    nullable_[node] = true;
    firstpos_[node] = make_bitset();
    lastpos_[node] = make_bitset();
}

void TreeFunctions::visit(const EndmarkerNode* node)
{
    nullable_[node] = false;
    firstpos_[node] = make_bitset(node->id());
    lastpos_[node] = make_bitset(node->id());
    symbols_[node->id()] = node->value();
    acceptpos_[node->id()] = node->name();
}

void followpos_graphviz(const TreeFunctions& functions)
{
    std::cout << "digraph {\n";
    std::string sep = "_";
    size_t n = 0;
    for (auto& pos : functions.followpos()) {
        std::cout << "\t" <<functions.symbols().at(n) +sep+ std::to_string(n+1) << " -> {";
        for (size_t i = 0, sz = pos.size(); i < sz; ++i) {
            if (pos.test(i)) {
                std::cout << "" << functions.symbols().at(i) +sep+ std::to_string(i+1) << " ";
            }
        }
        std::cout << "};\n";
        n++;
    }
    std::cout << "}\n";
}
