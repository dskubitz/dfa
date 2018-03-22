#ifndef LEXER_DERIVATIVE_H
#define LEXER_DERIVATIVE_H

#include <vector>
#include "Regex.h"

class Derivative : public RegexVisitor {
public:
    std::unique_ptr<Regex> derive(const Regex* tree, char da);
    std::unique_ptr<Regex> derive(const Regex* tree);
    void visit(const Regex* node) override;
    void visit(const Closure* node) override;
    void visit(const Concat* node) override;
    void visit(const Union* node) override;
    void visit(const Symbol* node) override;
    void visit(const Intersection* node) override;
    void visit(const Complement* node) override;
    void visit(const Empty* node) override;
    void visit(const Epsilon* node) override;
private:
    Regex* derive_impl(const Regex* tree);
    std::vector<Regex*> stack;
    Regex* evaluate(const Regex* node);
    char dA;
};

#endif //LEXER_DERIVATIVE_H
