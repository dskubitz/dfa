#ifndef LEXER_NULLABLE_H
#define LEXER_NULLABLE_H

#include <vector>
#include <Regexp.h>

class Nullable : public RegexVisitor {
public:
    bool evaluate(const RegexNode* regex);
    bool evaluate(const Regexp& regex);
private:
    void visit(const RegexNode* node) override;
    void visit(const Closure* node) override;
    void visit(const Concat* node) override;
    void visit(const Union* node) override;
    void visit(const Symbol* node) override;
    void visit(const Empty* node) override;
    void visit(const Intersection* node) override;
    void visit(const Complement* node) override;
    void visit(const Epsilon* node) override;
    std::vector<bool> stack;
};

bool is_nullable(const std::vector<Regexp>& rvec);

RegexNode* helper(const RegexNode* node);

#endif //LEXER_NULLABLE_H
