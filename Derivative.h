#ifndef LEXER_DERIVATIVE_H
#define LEXER_DERIVATIVE_H

#include <vector>
#include <memory>
#include <Regex.h>

class Derivative : public RegexVisitor {
public:
    Regex derive(const Regex& regex, char da);
    Regex derive(const Regex& regex);
    void visit(const RegexNode* node) override;
    void visit(const Closure* node) override;
    void visit(const Concat* node) override;
    void visit(const Union* node) override;
    void visit(const Symbol* node) override;
    void visit(const Intersection* node) override;
    void visit(const Complement* node) override;
    void visit(const Empty* node) override;
    void visit(const Epsilon* node) override;
private:
    RegexNode* derive_impl(const RegexNode* tree);
    std::vector<RegexNode*> stack;
    RegexNode* evaluate(const RegexNode* node);
    char dA;
};

#endif //LEXER_DERIVATIVE_H
