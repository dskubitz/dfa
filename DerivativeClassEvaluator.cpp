#include <numeric>
#include <algorithm>
#include "DerivativeClassEvaluator.h"

std::unordered_set<Bitset>
cross(const std::unordered_set<Bitset>& l, const std::unordered_set<Bitset>& r)
{
    if (l.empty()) return {r};
    if (r.empty()) return {l};

    std::unordered_set<Bitset> res;

    for (auto& setl : l)
        for (auto& setr : r)
            res.emplace(setl & setr);

    return res;
}

void DerivativeClassEvaluator::visit(const Regex::Node* node)
{
    node->accept(this);
}

void DerivativeClassEvaluator::visit(const Regex::Closure* closure)
{
    stack.push_back(evaluate(closure->expr()));
}

void DerivativeClassEvaluator::visit(const Regex::Concat* concat)
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

void DerivativeClassEvaluator::visit(const Regex::Union* anUnion)
{
    stack.push_back(
            cross(
                    evaluate(anUnion->left()),
                    evaluate(anUnion->right())));
}

void DerivativeClassEvaluator::visit(const Regex::Intersection* intersection)
{
    stack.push_back(
            cross(
                    evaluate(intersection->left()),
                    evaluate(intersection->right())));
}

void DerivativeClassEvaluator::visit(const Regex::Complement* complement)
{
    stack.push_back(evaluate(complement->expr()));
}

void DerivativeClassEvaluator::visit(const Regex::Symbol* symbol)
{
    Bitset set;
    set.flip();
    const Bitset& S = symbol->values();
    stack.emplace_back(std::unordered_set<Bitset>{S, set & ~S});
}

void DerivativeClassEvaluator::visit(const Regex::Epsilon* epsilon)
{
    (void) epsilon;
    Bitset set;
    set.flip();
    stack.emplace_back(std::unordered_set<Bitset>{set});
}

void DerivativeClassEvaluator::visit(const Regex::Empty* empty)
{
    (void) empty;
    Bitset set;
    set.flip();
    Bitset S;
    stack.emplace_back(std::unordered_set<Bitset>{S, set & ~S});
}

std::unordered_set<Bitset>
DerivativeClassEvaluator::evaluate(const Regex::Node* node)
{
    stack.clear();
    visit(node);
    std::unordered_set<Bitset> res = stack.back();
    stack.pop_back();
    return res;
}

std::unordered_set<Bitset>
DerivativeClassEvaluator::evaluate(const Regexp& regex)
{
    return evaluate(regex.get());
}

std::unordered_set<Bitset>
make_derivative_class(const std::vector<Regexp>& rvector)
{
    DerivativeClassEvaluator derivativeClass;
    std::unordered_set<Bitset> res;
    for (auto& i : rvector) {
        res = cross(res, derivativeClass.evaluate(i));
    }
    return res;
}
