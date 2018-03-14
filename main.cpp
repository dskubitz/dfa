#include <iostream>
#include <boost/dynamic_bitset.hpp>

#include "ASTNode.h"
#include "Printer.h"
#include "DFA.h"
#include "Parser.h"
#include "DFAFunctionCalculator.h"

ASTNode* unify(ASTNode* node)
{
    return node;
}

ASTNode* unify(ASTNode* left, ASTNode* right)
{
    return new UnionNode(left, right);
}

template<typename... Ts>
ASTNode* unify(ASTNode* left, ASTNode* right, Ts... rest)
{
    return unify(unify(left, right), unify(rest...));
}

ASTNode* add_endmarker(ASTNode* tree)
{
    return new CatNode(tree, new EndmarkerNode);
}

Parser parser;

ASTNode* create_regexes(const std::string& pattern)
{
    return parser.parse(pattern);
}

ASTNode* create_regexes(const std::string& pat1, const std::string& pat2)
{
    return unify(parser.parse(pat1), parser.parse(pat2));
}

template<class ... Ts>
ASTNode* create_regexes(const std::string& pat1, const std::string& pat2, Ts... rest)
{
    return unify(create_regexes(pat1, pat2), create_regexes(rest...));
}

template<class ... Ts>
std::unique_ptr<ASTNode> make_all(Ts... args)
{
    return std::unique_ptr<ASTNode>(add_endmarker(create_regexes(args...)));
}

int main()
{
    auto regex = make_all("(a|b)*abb");
    Printer{std::cout}.print(*regex);

    DFAFunctionCalculator calculator(*regex);
    int num = 0;
    for (auto& i : calculator.followpos_) {
        std::cout << calculator.symbols_[num] << ' ' << ++num << ' ' << i << '\n';
    }

    return 0;
}
