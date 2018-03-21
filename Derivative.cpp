#include "Derivative.h"

void Derivative::visit(const ASTNode* node)
{
    node->accept(this);
}

void Derivative::visit(const StarNode* node)
{
    stack.push_back(
            make_cat_regex(
                    evaluate(
                            node->expr()->clone()),
                    make_star_regex(
                            node->expr()->clone())));
}

void Derivative::visit(const CatNode* node)
{
    stack.push_back(
            make_union_regex(
                    make_cat_regex(
                            evaluate(node->left()),
                            node->right()->clone()),
                    make_cat_regex(
                            helper(node->left()),
                            evaluate(node->right()->clone()))));
}

void Derivative::visit(const UnionNode* node)
{
    stack.push_back(
            make_union_regex(
                    evaluate(node->left()->clone()),
                    evaluate(node->right()->clone())));
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

ASTNode* make_union_regex(ASTNode* left, ASTNode* right)
{
    if (dynamic_cast<EmptyNode*>(left)) {
        delete left;
        return right;
    } else if (dynamic_cast<EmptyNode*>(right)) {
        delete right;
        return left;
    } else if (*left == *right) {
        delete right;
        return left;
    } else {
        return new UnionNode(left, right);
    }
}

ASTNode* make_cat_regex(ASTNode* left, ASTNode* right)
{
    if (dynamic_cast<EmptyNode*>(left) || dynamic_cast<EmptyNode*>(right)) {
        delete right;
        return left;
    } else if (dynamic_cast<EpsilonNode*>(left)) {
        delete left;
        return right;
    } else if (dynamic_cast<EpsilonNode*>(right)) {
        delete right;
        return left;
    } else {
        return new CatNode(left, right);
    }
}

ASTNode* make_star_regex(ASTNode* expr)
{
    if (dynamic_cast<EmptyNode*>(expr)) {
        delete expr;
        return new EpsilonNode;
    } else if (dynamic_cast<EpsilonNode*>(expr)) {
        return expr;
    } else if (auto p = dynamic_cast<StarNode*>(expr)) {
        ASTNode* q = p->expr()->clone();
        delete p;
        return make_star_regex(q);
    } else {
        return new StarNode(expr);
    }
}

ASTNode* helper(const ASTNode* node)
{
    if (Nullable{}.evaluate(node))
        return new EpsilonNode;
    else
        return new EmptyNode;
}
