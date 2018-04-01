#ifndef LEXER_DERIVATIVECLASS_H
#define LEXER_DERIVATIVECLASS_H

#include <vector>
#include <unordered_set>
#include "Regexp.h"
#include "NullableEvaluator.h"

class DerivativeClassEvaluator : Regex::Visitor {
public:
    std::unordered_set<Bitset> evaluate(const Regexp& regex);
private:
    void visit(const Regex::Node* node) override;
    void visit(const Regex::Closure* closure) override;
    void visit(const Regex::Concat* concat) override;
    void visit(const Regex::Union* anUnion) override;
    void visit(const Regex::Intersection* intersection) override;
    void visit(const Regex::Complement* complement) override;
    void visit(const Regex::Symbol* symbol) override;
    void visit(const Regex::Epsilon* epsilon) override;
    void visit(const Regex::Empty* empty) override;
    std::unordered_set<Bitset> evaluate(const Regex::Node* node);
    NullableEvaluator nullable;
    std::vector<std::unordered_set<Bitset>> stack;
};

std::unordered_set<Bitset>
make_derivative_class(const std::vector<Regexp>& rvector);

std::unordered_set<Bitset>
cross(const std::unordered_set<Bitset>& l, const std::unordered_set<Bitset>& r);
#endif //LEXER_DERIVATIVECLASS_H
