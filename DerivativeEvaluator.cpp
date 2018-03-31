#include <DerivativeEvaluator.h>
#include <NullableEvaluator.h>
#include <algorithm>

void DerivativeEvaluator::visit(const Regex::Node* node)
{
    node->accept(this);
}

void DerivativeEvaluator::visit(const Regex::Closure* node)
{
    stack.push_back(
            make_cat(
                    evaluate(
                            node->expr()),
                    make_star(
                            node->expr()->clone())));
}

void DerivativeEvaluator::visit(const Regex::Concat* node)
{
    stack.push_back(
            make_union(
                    make_cat(
                            evaluate(node->left()),
                            node->right()->clone()),
                    make_cat(
                            helper(node->left()),
                            evaluate(node->right()))));
}

void DerivativeEvaluator::visit(const Regex::Union* node)
{
    stack.push_back(
            make_union(
                    evaluate(node->left()),
                    evaluate(node->right())));
}

void DerivativeEvaluator::visit(const Regex::Intersection* node)
{
    stack.push_back(
            make_intersection(
                    evaluate(node->left()),
                    evaluate(node->right())));
}

void DerivativeEvaluator::visit(const Regex::Complement* node)
{
    stack.push_back(
            make_complement(
                    evaluate(node->expr()->clone())));
}

void DerivativeEvaluator::visit(const Regex::Symbol* node)
{
    if (node->values().test(dA))
        stack.push_back(new Regex::Epsilon);
    else
        stack.push_back(new Regex::Empty);
}

void DerivativeEvaluator::visit(const Regex::Epsilon* node)
{
    stack.push_back(new Regex::Empty);
}

void DerivativeEvaluator::visit(const Regex::Empty* node)
{
    stack.push_back(new Regex::Empty);
}

Regex::Node* DerivativeEvaluator::derive_impl(const Regex::Node* tree)
{
    return evaluate(tree);
}

Regex::Node* DerivativeEvaluator::evaluate(const Regex::Node* node)
{
    visit(node);
    Regex::Node* res = stack.back();
    stack.pop_back();
    return res;
}

Regexp DerivativeEvaluator::derive(const Regexp& regex, char da)
{
    stack.clear();
    dA = da;
    return Regexp(derive_impl(regex.get()));
}

std::vector<Regexp> make_derivative(const std::vector<Regexp>& rvector, char da)
{
    DerivativeEvaluator D;
    std::vector<Regexp> res;
    for (auto& re : rvector) {
        res.push_back(D.derive(re, da));
    }
    return res;
}
