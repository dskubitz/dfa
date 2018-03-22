#ifndef LEXER_NULLABLE_H
#define LEXER_NULLABLE_H

#include <vector>
#include "Regex.h"

class Nullable : public RegexVisitor {
public:
    void visit(const Regex* node) override;
    void visit(const Closure* node) override;
    void visit(const Concat* node) override;
    void visit(const Union* node) override;
    void visit(const Symbol* node) override;
    void visit(const Empty* node) override;
    void visit(const Intersection* node) override;
    void visit(const Complement* node) override;
    void visit(const Epsilon* node) override;
    bool evaluate(const Regex* node);
private:
    std::vector<bool> stack;
};

Regex* helper(const Regex* node);

#endif //LEXER_NULLABLE_H
