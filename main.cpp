#include <iostream>
#include <boost/dynamic_bitset.hpp>

#include "ASTNode.h"
#include "Printer.h"
#include "DFA.h"
#include "Parser.h"
#include "DFAFunctionCalculator.h"

ASTNode* add_endmarker(ASTNode* tree)
{
    return new CatNode(tree, new EndmarkerNode);
}

ASTNode* unify(ASTNode* node)
{
    return node;
}

ASTNode* unify(ASTNode* left, ASTNode* right)
{
    return new UnionNode(unify(left), unify(right));
}

template<typename... Ts>
ASTNode* unify(ASTNode* left, ASTNode* right, Ts... rest)
{
    return unify(unify(left, right), unify(rest...));
}

Parser parser;

ASTNode* create_regexes(const std::string& pattern)
{
    return add_endmarker(parser.parse(pattern));
}

ASTNode* create_regexes(const std::string& pat1, const std::string& pat2)
{
    return unify(create_regexes(pat1), create_regexes(pat2));
}

template<class ... Ts>
ASTNode*
create_regexes(const std::string& pat1, const std::string& pat2, Ts... rest)
{
    return unify(create_regexes(pat1, pat2), create_regexes(rest...));
}

template<class ... Ts>
std::unique_ptr<ASTNode> make_all(Ts... args)
{
    return std::unique_ptr<ASTNode>(create_regexes(args...));
}
void test(const TransitionTable& table)
{
}

void test(const TransitionTable& table, const std::string& s)
{
    int state = 0;
    for (char ch : s) {
        state = table[state][ch];
        std::cout << state << ' ' << ch << '\n';
    }
    std::cout << '\n';
}

template<typename...Args>
void test(const TransitionTable& table, const std::string& s, Args... args)
{
    test(table, s);
    test(table, args...);
}

int main()
{
    auto regex = make_all("and", "class", "false", "fun", "for", "if", "then", "else", "[A-Za-z_][A-Za-z0-9_]*", "[1-9][0-9]*");
    Printer{std::cout}.print(*regex);
    auto dtrans = make_transition_table(*regex);
    std::cout << dtrans.size() << '\n';
    test(dtrans, "and ", "class ", "false ", "fun ", "for ", "if ", "then ", "else ", "hello123 ", "_Hello__world__ ", "123hello ", "__123 ", "123__ ");

    return 0;
}
