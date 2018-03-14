#ifndef LEXER_PRETTYPRINTER_H
#define LEXER_PRETTYPRINTER_H

#include <ostream>

#include "ASTNode.h"

class Printer : public ASTVisitor {
public:
    explicit Printer(std::ostream& os);

    void print(ASTNode& tree);

    void visit(StarNode& node) override;
    void visit(CatNode& node) override;
    void visit(UnionNode& node) override;
    void visit(CharNode& node) override;
    void visit(EpsilonNode& node) override;
    void visit(EndmarkerNode& node) override;
private:
    std::ostream& os_;
};

std::ostream& operator<<(std::ostream& os, const ASTNode& tree);

#endif //LEXER_PRETTYPRINTER_H
