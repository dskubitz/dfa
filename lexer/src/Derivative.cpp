#include "lexer/Derivative.h"
#include <algorithm>

void Derivative::visit(const Regex::Node* node)
{
    node->accept(this);
}

void Derivative::visit(const Regex::Closure* node)
{
    stack.push_back(
            make_concatenation(
                    evaluate(
                            node->expr()),
                    make_closure(
                            node->expr()->clone())));
}

void Derivative::visit(const Regex::Concat* node)
{
    stack.push_back(
            make_union(
                    make_concatenation(
                            evaluate(node->left()),
                            node->right()->clone()),
                    make_concatenation(
                            helper(node->left()),
                            evaluate(node->right()))));
}

void Derivative::visit(const Regex::Union* node)
{
    stack.push_back(
            make_union(
                    evaluate(node->left()),
                    evaluate(node->right())));
}

void Derivative::visit(const Regex::Intersection* node)
{
    stack.push_back(
            make_intersection(
                    evaluate(node->left()),
                    evaluate(node->right())));
}

void Derivative::visit(const Regex::Complement* node)
{
    stack.push_back(
            make_complement(
                    evaluate(node->expr()->clone())));
}

void Derivative::visit(const Regex::Symbol* node)
{
    if (node->values().test(dA))
        stack.push_back(new Regex::Epsilon);
    else
        stack.push_back(new Regex::Empty);
}

void Derivative::visit(const Regex::Epsilon* node)
{
    stack.push_back(new Regex::Empty);
}

void Derivative::visit(const Regex::Empty* node)
{
    stack.push_back(new Regex::Empty);
}

Regex::Node* Derivative::derive_impl(const Regex::Node* tree)
{
    return evaluate(tree);
}

Regex::Node* Derivative::evaluate(const Regex::Node* node)
{
    visit(node);
    Regex::Node* res = stack.back();
    stack.pop_back();
    return res;
}

Regexp Derivative::derive(const Regexp& regex, char da)
{
    stack.clear();
    dA = da;
    return Regexp(derive_impl(regex.get()));
}

std::vector<Regexp> make_derivative(const std::vector<Regexp>& rvector, char da)
{
    Derivative D;
    std::vector<Regexp> res;
    for (auto& re : rvector) {
        res.push_back(D.derive(re, da));
    }
    return res;
}

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

void DerivativeClass::visit(const Regex::Node* node)
{
    node->accept(this);
}

void DerivativeClass::visit(const Regex::Closure* closure)
{
    stack.push_back(evaluate(closure->expr()));
}

void DerivativeClass::visit(const Regex::Concat* concat)
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

void DerivativeClass::visit(const Regex::Union* anUnion)
{
    stack.push_back(
            cross(
                    evaluate(anUnion->left()),
                    evaluate(anUnion->right())));
}

void DerivativeClass::visit(const Regex::Intersection* intersection)
{
    stack.push_back(
            cross(
                    evaluate(intersection->left()),
                    evaluate(intersection->right())));
}

void DerivativeClass::visit(const Regex::Complement* complement)
{
    stack.push_back(evaluate(complement->expr()));
}

void DerivativeClass::visit(const Regex::Symbol* symbol)
{
    Bitset set;
    set.flip();
    const Bitset& S = symbol->values();
    stack.emplace_back(std::unordered_set<Bitset>{S, set & ~S});
}

void DerivativeClass::visit(const Regex::Epsilon* epsilon)
{
    (void) epsilon;
    Bitset set;
    set.flip();
    stack.emplace_back(std::unordered_set<Bitset>{set});
}

void DerivativeClass::visit(const Regex::Empty* empty)
{
    (void) empty;
    Bitset set;
    set.flip();
    Bitset S;
    stack.emplace_back(std::unordered_set<Bitset>{S, set & ~S});
}

std::unordered_set<Bitset>
DerivativeClass::evaluate(const Regex::Node* node)
{
    stack.clear();
    visit(node);
    std::unordered_set<Bitset> res = stack.back();
    stack.pop_back();
    return res;
}

std::unordered_set<Bitset>
DerivativeClass::evaluate(const Regexp& regex)
{
    return evaluate(regex.get());
}

std::unordered_set<Bitset>
make_derivative_class(const std::vector<Regexp>& rvector)
{
    DerivativeClass derivativeClass;
    std::unordered_set<Bitset> res;
    for (auto& i : rvector) {
        res = cross(res, derivativeClass.evaluate(i));
    }
    return res;
}

void Nullable::visit(const Regex::Node* node)
{
    node->accept(this);
}

void Nullable::visit(const Regex::Closure* node)
{
    stack.push_back(true);
}

void Nullable::visit(const Regex::Concat* node)
{
    stack.push_back(evaluate(node->left()) && evaluate(node->right()));
}

void Nullable::visit(const Regex::Union* node)
{
    stack.push_back(evaluate(node->left()) || evaluate(node->right()));
}

void Nullable::visit(const Regex::Symbol* node)
{
    stack.push_back(false);
}

void Nullable::visit(const Regex::Epsilon* node)
{
    stack.push_back(true);
}

void Nullable::visit(const Regex::Empty* node)
{
    stack.push_back(false);
}

void Nullable::visit(const Regex::Intersection* node)
{
    stack.push_back(evaluate(node->left()) && evaluate(node->right()));
}

void Nullable::visit(const Regex::Complement* node)
{
    stack.push_back(!evaluate(node->expr()));
}

bool Nullable::evaluate(const Regex::Node* regex)
{
    stack.clear();
    visit(regex);
    bool res = stack.back();
    stack.pop_back();
    return res;
}

bool Nullable::evaluate(const Regexp& regex)
{
    return evaluate(regex.get());
}

Regex::Node* helper(const Regex::Node* node)
{
    if (Nullable{}.evaluate(node))
        return new Regex::Epsilon;
    else
        return new Regex::Empty;
}

