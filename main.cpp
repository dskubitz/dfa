#include <iostream>

#include "ASTNode.h"
#include "DFA.h"
#include "Parser.h"
#include "DFAFunctionCalculator.h"

ASTNode* unify(ASTNode* left, ASTNode* right)
{
    return new UnionNode(left, right);
}

Parser parser;

ASTNode* parse(const std::string& pattern)
{
    return new CatNode(parser.parse(pattern), new EndmarkerNode(pattern));
}

template<class Iter,
         class = typename std::enable_if<
                 std::is_convertible<
                         typename std::iterator_traits<Iter>::iterator_category,
                         std::input_iterator_tag>::value>::type>
ASTNode* create_regexes(Iter b, Iter e)
{
    if (b == e)
        return nullptr;

    ASTNode* node = parse(*b++);
    for (; b != e; ++b) {
        node = unify(node, parse(*b));
    }
    return node;
}

std::unique_ptr<ASTNode> make_all(std::initializer_list<const char*> list)
{
    return std::unique_ptr<ASTNode>(create_regexes(list.begin(), list.end()));
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
    auto regex = make_all({"if", "then", "else",
                           "[A-Za-z_][A-Za-z0-9_]*", "[1-9][0-9]*"
                          });
    DFAFunctionCalculator calc(regex.get());
    auto dtrans = make_transition_table(calc);

    std::cout << dtrans.size() << '\n';

    test(dtrans, "AahcdefghijKLMNOP_qRs10tuvwxzy ", "if ", "then ", "else ");

    return 0;
}
