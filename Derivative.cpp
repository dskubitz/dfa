#include <Derivative.h>
#include <Nullable.h>

void Derivative::visit(const Regex* node)
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
    if (node->value() == dA)
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

Regex* Derivative::derive_impl(const Regex* tree)
{
    return evaluate(tree);
}

std::unique_ptr<Regex> Derivative::derive(const Regex* tree, char da)
{
    stack.clear();
    dA = da;
    return std::unique_ptr<Regex>(derive_impl(tree));
}

Regex* Derivative::evaluate(const Regex* node)
{
    visit(node);
    Regex* res = stack.back();
    stack.pop_back();
    return res;
}

// Overload is derivative w.r.t empty string
std::unique_ptr<Regex> Derivative::derive(const Regex* tree)
{
    stack.clear();
    dA = 0;
    return std::unique_ptr<Regex>(tree->clone());
}
