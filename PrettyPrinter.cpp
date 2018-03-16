#include "PrettyPrinter.h"

PrettyPrinter::PrettyPrinter(std::ostream& os)
        :os_(os)
{
}

void PrettyPrinter::visit(const StarNode* node)
{
    os_ << "(star ";
    node->expr()->accept(this);
    os_ << ")";
}

void PrettyPrinter::visit(const CatNode* node)
{
    os_ << "(cat ";
    node->left()->accept(this);
    os_ << " ";
    node->right()->accept(this);
    os_ << ")";
}

void PrettyPrinter::visit(const UnionNode* node)
{
    os_ << "(union ";
    node->left()->accept(this);
    os_ << " ";
    node->right()->accept(this);
    os_ << ")";
}

void PrettyPrinter::visit(const CharNode* node)
{
    os_ << node->value();
}

void PrettyPrinter::visit(const EpsilonNode* node)
{
    os_ << "\u03B5";
}

void PrettyPrinter::visit(const EndmarkerNode* node)
{
    os_ << '#';
}

void PrettyPrinter::print(const ASTNode* tree)
{
    tree->accept(this);
    os_ << '\n';
}

void PrettyPrinter::visit(const ASTNode* node)
{
    node->accept(this);
}
