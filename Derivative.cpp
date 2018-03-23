#include <Derivative.h>
#include <Nullable.h>

void Derivative::visit(const RegexNode* node)
{
    node->accept(this);
}

void Derivative::visit(const Closure* node)
{
    stack.push_back(
            make_cat(
                    evaluate(
                            node->expr()->clone()),
                    make_star(
                            node->expr()->clone())));
}

void Derivative::visit(const Concat* node)
{
    stack.push_back(
            make_union(
                    make_cat(
                            evaluate(node->left()),
                            node->right()->clone()),
                    make_cat(
                            helper(node->left()),
                            evaluate(node->right()->clone()))));
}

void Derivative::visit(const Union* node)
{
    stack.push_back(
            make_union(
                    evaluate(node->left()->clone()),
                    evaluate(node->right()->clone())));
}

void Derivative::visit(const Intersection* node)
{
    stack.push_back(
            make_intersection(
                    evaluate(node->left()->clone()),
                    evaluate(node->right()->clone())));
}

void Derivative::visit(const Complement* node)
{
    stack.push_back(
            make_complement(
                    evaluate(node->expr()->clone())));
}

void Derivative::visit(const Symbol* node)
{
    if (node->values().test(dA))
        stack.push_back(new Epsilon);
    else
        stack.push_back(new Empty);
}

void Derivative::visit(const Epsilon* node)
{
    stack.push_back(new Empty);
}

void Derivative::visit(const Empty* node)
{
    stack.push_back(new Empty);
}

RegexNode* Derivative::derive_impl(const RegexNode* tree)
{
    return evaluate(tree);
}

Regex Derivative::derive(const Regex& regex, char da)
{
    stack.clear();
    dA = da;
    return Regex(derive_impl(regex.get()));
}

RegexNode* Derivative::evaluate(const RegexNode* node)
{
    visit(node);
    RegexNode* res = stack.back();
    stack.pop_back();
    return res;
}

// Overload is derivative w.r.t empty string
Regex Derivative::derive(const Regex& regex)
{
    stack.clear();
    dA = 0;
    return regex;
}
