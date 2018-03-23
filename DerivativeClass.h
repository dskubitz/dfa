#ifndef LEXER_DERIVATIVECLASS_H
#define LEXER_DERIVATIVECLASS_H

#include <vector>
#include "Regex.h"

class DerivativeClass : RegexVisitor {
public:
private:
    void visit(const RegexNode* node) override;
    void visit(const Closure* closure) override;
    void visit(const Concat* concat) override;
    void visit(const Union* anUnion) override;
    void visit(const Intersection* intersection) override;
    void visit(const Complement* complement) override;
    void visit(const Symbol* symbol) override;
    void visit(const Epsilon* epsilon) override;
    void visit(const Empty* empty) override;
private:
    std::vector<RegexNode*> stack;
    RegexNode* evaluate(const RegexNode* node);
};

#endif //LEXER_DERIVATIVECLASS_H
