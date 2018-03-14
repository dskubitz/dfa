#include <iostream>
#include <boost/dynamic_bitset.hpp>

#include "ASTNode.h"
#include "Printer.h"
#include "DFA.h"
#include "Parser.h"
#include "TransitionTable.h"
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

int main()
{
    Parser parser;

    std::string pat1("if");
    std::string pat2("then");
    ASTNode* regex1 = parser.parse(pat1);
    ASTNode* regex2 = parser.parse(pat2);
    auto regex = std::unique_ptr<ASTNode>(add_endmarker(unify(regex1, regex2)));
    std::cout << *regex << '\n';

    DFAFunctionCalculator calculator(*regex);
    int num = 0;
    for (auto& i : calculator.followpos_) {
        std::cout << ++num << ' ' << i << '\n';
    }

//    print_pos_sets(*regex3);

    return 0;
}
