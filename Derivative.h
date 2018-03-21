//
// Created by devan on 3/21/18.
//

#ifndef LEXER_DERIVATIVE_H
#define LEXER_DERIVATIVE_H

#include <vector>
#include "ASTNode.h"

class Nullable : public ASTVisitor {
public:
    void visit(const ASTNode* node) override;
    void visit(const StarNode* node) override;
    void visit(const CatNode* node) override;
    void visit(const UnionNode* node) override;
    void visit(const CharNode* node) override;
    void visit(const EmptyNode* node) override;
    void visit(const IntersectionNode* node) override;
    void visit(const ComplementNode* node) override;
    void visit(const EpsilonNode* node) override;
    bool evaluate(const ASTNode* node);
private:
    std::vector<bool> stack;
};

ASTNode* helper(const ASTNode* node);

class Derivative : public ASTVisitor {
public:
    std::unique_ptr<ASTNode> derive(const ASTNode* tree, char da);
    std::unique_ptr<ASTNode> derive(const ASTNode* tree);
    void visit(const ASTNode* node) override;
    void visit(const StarNode* node) override;
    void visit(const CatNode* node) override;
    void visit(const UnionNode* node) override;
    void visit(const CharNode* node) override;
    void visit(const IntersectionNode* node) override;
    void visit(const ComplementNode* node) override;
    void visit(const EmptyNode* node) override;
    void visit(const EpsilonNode* node) override;
private:
    ASTNode* derive_impl(const ASTNode* tree);
    std::vector<ASTNode*> stack;
    ASTNode* evaluate(const ASTNode* node);
    char dA;
};


#endif //LEXER_DERIVATIVE_H
