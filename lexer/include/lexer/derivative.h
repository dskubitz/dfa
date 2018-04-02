#ifndef LEXER_DERIVATIVE_H
#define LEXER_DERIVATIVE_H

#include <vector>
#include <memory>
#include <unordered_set>

#include <lexer/regexp.h>

class derivative : public Regex::Visitor {
public:
    regexp derive(const regexp& regex, char da);
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

class Nullable : public Regex::Visitor {
public:
    bool evaluate(const Regex::Node* regex);
    bool evaluate(const regexp& regex);
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

std::vector<regexp>
make_derivative(const std::vector<regexp>& rvector, char da);

class DerivativeClass : Regex::Visitor {
public:
    std::unordered_set<Bitset> evaluate(const regexp& regex);
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
    Nullable nullable;
    std::vector<std::unordered_set<Bitset>> stack;
};

std::unordered_set<Bitset>
make_derivative_class(const std::vector<regexp>& rvector);

std::unordered_set<Bitset>
cross(const std::unordered_set<Bitset>& l, const std::unordered_set<Bitset>& r);

#endif //LEXER_DERIVATIVE_H
