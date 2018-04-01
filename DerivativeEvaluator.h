#ifndef LEXER_DERIVATIVE_H
#define LEXER_DERIVATIVE_H

#include <vector>
#include <memory>
#include <Regexp.h>

class DerivativeEvaluator : public Regex::Visitor {
public:
    Regexp derive(const Regexp& regex, char da);
private:
    void visit(const Regex::Node* node) override;
    void visit(const Regex::Closure* node) override;
    void visit(const Regex::Concat* node) override;
    void visit(const Regex::Union* node) override;
    void visit(const Regex::Symbol* node) override;
    void visit(const Regex::Intersection* node) override;
    void visit(const Regex::Complement* node) override;
    void visit(const Regex::Empty* node) override;
    void visit(const Regex::Epsilon* node) override;
    Regex::Node* derive_impl(const Regex::Node* tree);
    std::vector<Regex::Node*> stack;
    Regex::Node* evaluate(const Regex::Node* node);
    char dA;
};

std::vector<Regexp> make_derivative(const std::vector<Regexp>& rvector, char da);

#endif //LEXER_DERIVATIVE_H
