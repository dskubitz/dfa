#ifndef LEXER_NULLABLE_H
#define LEXER_NULLABLE_H

#include <vector>
#include <Regexp.h>

class NullableEvaluator : public Regex::Visitor {
public:
    bool evaluate(const Regex::Node* regex);
    bool evaluate(const Regexp& regex);
private:
    void visit(const Regex::Node* node) override;
    void visit(const Regex::Closure* node) override;
    void visit(const Regex::Concat* node) override;
    void visit(const Regex::Union* node) override;
    void visit(const Regex::Symbol* node) override;
    void visit(const Regex::Empty* node) override;
    void visit(const Regex::Intersection* node) override;
    void visit(const Regex::Complement* node) override;
    void visit(const Regex::Epsilon* node) override;
    std::vector<bool> stack;
};

Regex::Node* helper(const Regex::Node* node);

#endif //LEXER_NULLABLE_H
