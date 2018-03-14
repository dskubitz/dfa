#include "Printer.h"

Printer::Printer(std::ostream& os)
        :os_(os)
{
}

void Printer::print(ASTNode& tree)
{
    tree.accept(*this);
    os_ << '\n';
}

void Printer::visit(StarNode& node)
{
    os_ << "(star ";
    node.expr()->accept(*this);
    os_ << ")";
}

void Printer::visit(CatNode& node)
{
    os_ << "(cat ";
    node.left()->accept(*this);
    os_ << " ";
    node.right()->accept(*this);
    os_ << ")";
}

void Printer::visit(UnionNode& node)
{
    os_ << "(union ";
    node.left()->accept(*this);
    os_ << " ";
    node.right()->accept(*this);
    os_ << ")";
}

void Printer::visit(CharNode& node)
{
    os_ << node.value();
}

void Printer::visit(EpsilonNode& node)
{
    os_ << "\u03B5";
}

void Printer::visit(EndmarkerNode& node)
{
    os_ << '#';
}

std::ostream& operator<<(std::ostream& os, const ASTNode& node)
{
    Printer printer(os);
    printer.print(const_cast<ASTNode&>(node));
    return os;
}
