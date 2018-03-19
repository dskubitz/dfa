#include "PrettyPrinter.h"

PrettyPrinter::PrettyPrinter(std::ostream& os)
        :os_(os)
{
}

void PrettyPrinter::visit(const StarNode* node)
{
    os_ << "(star ";
    visit(node->expr());
    os_ << ")";
}

void PrettyPrinter::visit(const CatNode* node)
{
    os_ << "(cat ";
    visit(node->left());
    os_ << " ";
    visit(node->right());
    os_ << ")";
}

void PrettyPrinter::visit(const UnionNode* node)
{
    os_ << "(union ";
    visit(node->left());
    os_ << " ";
    visit(node->right());
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
    visit(tree);
    os_ << '\n';
}

void PrettyPrinter::visit(const ASTNode* node)
{
    node->accept(this);
}
