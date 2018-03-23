#include "DerivativeClass.h"

std::unordered_set<Bitset>
cross(const std::unordered_set<Bitset>& l, const std::unordered_set<Bitset>& r)
{
    std::unordered_set<Bitset> res;
    for (auto& setl : l) {
        for (auto& setr : r) {
            res.emplace(setl & setr);
        }
    }
    return res;
}

void DerivativeClass::visit(const RegexNode* node)
{
    node->accept(this);
}

void DerivativeClass::visit(const Closure* closure)
{
    stack.push_back(evaluate(closure->expr()));
}

void DerivativeClass::visit(const Concat* concat)
{
    if (nullable.evaluate(concat->left())) {
        stack.push_back(
                cross(
                        evaluate(concat->left()),
                        evaluate(concat->right())));
    } else {
        stack.push_back(evaluate(concat->left()));
    }
}

void DerivativeClass::visit(const Union* anUnion)
{
    stack.push_back(
            cross(
                    evaluate(anUnion->left()),
                    evaluate(anUnion->right())));
}

void DerivativeClass::visit(const Intersection* intersection)
{
    stack.push_back(
            cross(
                    evaluate(intersection->left()),
                    evaluate(intersection->right())));
}

void DerivativeClass::visit(const Complement* complement)
{
    stack.push_back(evaluate(complement->expr()));
}

void DerivativeClass::visit(const Symbol* symbol)
{
    Bitset set;
    set.flip();
    const Bitset& S = symbol->values();
    stack.emplace_back(std::unordered_set<Bitset>{S, set & ~S});
}

void DerivativeClass::visit(const Epsilon* epsilon)
{
    (void) epsilon;
    Bitset set;
    set.flip();
    stack.emplace_back(std::unordered_set<Bitset>{set});
}

void DerivativeClass::visit(const Empty* empty)
{
    (void) empty;
    Bitset set;
    set.flip();
    Bitset S;
    stack.emplace_back(std::unordered_set<Bitset>{S, set & ~S});
}

std::unordered_set<Bitset> DerivativeClass::evaluate(const RegexNode* node)
{
    visit(node);
    std::unordered_set<Bitset> res = stack.back();
    stack.pop_back();
    return res;
}

std::unordered_set<Bitset> DerivativeClass::evaluate(const Regex& regex)
{
    return evaluate(regex.get());
}
