#ifndef LEXER_PRETTYPRINTER_H
#define LEXER_PRETTYPRINTER_H

#include <ostream>

#include "ASTNode.h"

class PrettyPrinter : public ASTVisitor {
public:
    explicit PrettyPrinter(std::ostream& os);

    void print(const ASTNode* tree);

    void visit(const ASTNode* node) override;

    void visit(const StarNode* node) override;
    void visit(const CatNode* node) override;
    void visit(const UnionNode* node) override;
    void visit(const CharNode* node) override;
    void visit(const EpsilonNode* node) override;
    void visit(const EndmarkerNode* node) override;
private:
    std::ostream& os_;
};

#endif //LEXER_PRETTYPRINTER_H
