#ifndef LEXER_DERIVATIVECLASS_H
#define LEXER_DERIVATIVECLASS_H

#include <vector>
#include <unordered_set>
#include "Regex.h"
#include "Nullable.h"

class DerivativeClass : RegexVisitor {
public:
    std::unordered_set<Bitset> evaluate(const Regex& regex);
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
    std::unordered_set<Bitset> evaluate(const RegexNode* node);
    Nullable nullable;
    std::vector<std::unordered_set<Bitset>> stack;
};

#endif //LEXER_DERIVATIVECLASS_H
