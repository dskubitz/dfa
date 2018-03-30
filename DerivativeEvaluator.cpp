#include <DerivativeEvaluator.h>
#include <NullableEvaluator.h>
#include <algorithm>

void DerivativeEvaluator::visit(const RegexNode* node)
{
    node->accept(this);
}

void DerivativeEvaluator::visit(const Closure* node)
{
    stack.push_back(
            make_cat(
                    evaluate(
                            node->expr()),
                    make_star(
                            node->expr()->clone())));
}

void DerivativeEvaluator::visit(const Concat* node)
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

void DerivativeEvaluator::visit(const Union* node)
{
    stack.push_back(
            make_union(
                    evaluate(node->left()),
                    evaluate(node->right())));
}

void DerivativeEvaluator::visit(const Intersection* node)
{
    stack.push_back(
            make_intersection(
                    evaluate(node->left()),
                    evaluate(node->right())));
}

void DerivativeEvaluator::visit(const Complement* node)
{
    stack.push_back(
            make_complement(
                    evaluate(node->expr()->clone())));
}

void DerivativeEvaluator::visit(const Symbol* node)
{
    if (node->values().test(dA))
        stack.push_back(new Epsilon);
    else
        stack.push_back(new Empty);
}

void DerivativeEvaluator::visit(const Epsilon* node)
{
    stack.push_back(new Empty);
}

void DerivativeEvaluator::visit(const Empty* node)
{
    stack.push_back(new Empty);
}

RegexNode* DerivativeEvaluator::derive_impl(const RegexNode* tree)
{
    return evaluate(tree);
}

RegexNode* DerivativeEvaluator::evaluate(const RegexNode* node)
{
    visit(node);
    RegexNode* res = stack.back();
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
