#ifndef LEXER_DERIVATIVE_H
#define LEXER_DERIVATIVE_H

#include <vector>
#include <memory>
#include <Regexp.h>

class Derivative : public RegexVisitor {
public:
    Regexp derive(const Regexp& regex, char da);
    Regexp derive(const Regexp& regex);
private:
    void visit(const RegexNode* node) override;
    void visit(const Closure* node) override;
    void visit(const Concat* node) override;
    void visit(const Union* node) override;
    void visit(const Symbol* node) override;
    void visit(const Intersection* node) override;
    void visit(const Complement* node) override;
    void visit(const Empty* node) override;
    void visit(const Epsilon* node) override;
    RegexNode* derive_impl(const RegexNode* tree);
    std::vector<RegexNode*> stack;
    RegexNode* evaluate(const RegexNode* node);
    char dA;
};

std::vector<Regexp> make_derivative(const std::vector<Regexp>& rvector, char da);

#endif //LEXER_DERIVATIVE_H
