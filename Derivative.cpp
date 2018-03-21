#include "Derivative.h"

void Derivative::visit(const ASTNode* node)
{
    node->accept(this);
}

void Derivative::visit(const StarNode* node)
{
    stack.push_back(
            make_cat(
                    evaluate(
                            node->expr()->clone()),
                    make_star(
                            node->expr()->clone())));
}

void Derivative::visit(const CatNode* node)
{
    stack.push_back(
            make_union(
                    make_cat(
                            evaluate(node->left()),
                            node->right()->clone()),
                    make_cat(
                            helper(node->left()),
                            evaluate(node->right()->clone()))));
}

void Derivative::visit(const UnionNode* node)
{
    stack.push_back(
            make_union(
                    evaluate(node->left()->clone()),
                    evaluate(node->right()->clone())));
}

void Derivative::visit(const IntersectionNode* node)
{

}

void Derivative::visit(const ComplementNode* node)
{

}

void Derivative::visit(const CharNode* node)
{
    if (node->value() == dA)
        stack.push_back(new EpsilonNode);
    else
        stack.push_back(new EmptyNode);
}

void Derivative::visit(const EpsilonNode* node)
{
    stack.push_back(new EmptyNode);
}

void Derivative::visit(const EmptyNode* node)
{
    stack.push_back(new EmptyNode);
}

ASTNode* Derivative::derive_impl(const ASTNode* tree)
{
    return evaluate(tree);
}

std::unique_ptr<ASTNode> Derivative::derive(const ASTNode* tree, char da)
{
    stack.clear();
    dA = da;
    return std::unique_ptr<ASTNode>(derive_impl(tree));
}

ASTNode* Derivative::evaluate(const ASTNode* node)
{
    visit(node);
    ASTNode* res = stack.back();
    stack.pop_back();
    return res;
}

// Overload is derivative w.r.t empty string
std::unique_ptr<ASTNode> Derivative::derive(const ASTNode* tree)
{
    stack.clear();
    dA = 0;
    return std::unique_ptr<ASTNode>(tree->clone());
}



void Nullable::visit(const ASTNode* node)
{
    node->accept(this);
}

void Nullable::visit(const StarNode* node)
{
    stack.push_back(true);
}

void Nullable::visit(const CatNode* node)
{
    stack.push_back(evaluate(node->left()) && evaluate(node->right()));
}

void Nullable::visit(const UnionNode* node)
{
    stack.push_back(evaluate(node->left()) || evaluate(node->right()));
}

void Nullable::visit(const CharNode* node)
{
    stack.push_back(false);
}

void Nullable::visit(const EpsilonNode* node)
{
    stack.push_back(true);
}

void Nullable::visit(const EmptyNode* node)
{
    stack.push_back(false);
}

bool Nullable::evaluate(const ASTNode* node)
{
    visit(node);
    bool res = stack.back();
    stack.pop_back();
    return res;
}

void Nullable::visit(const IntersectionNode* node)
{
    stack.push_back(evaluate(node->left()) && evaluate(node->right()));
}

void Nullable::visit(const ComplementNode* node)
{
    stack.push_back(!evaluate(node->expr()));
}



ASTNode* helper(const ASTNode* node)
{
    if (Nullable{}.evaluate(node))
        return new EpsilonNode;
    else
        return new EmptyNode;
}
