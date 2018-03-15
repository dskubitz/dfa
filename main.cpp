#include <iostream>

#include "ASTNode.h"
#include "TransitionTable.h"
#include "Parser.h"
#include "TreeFunctions.h"

static Parser parser;

ASTNode* unify(ASTNode* left, ASTNode* right)
{
    return new UnionNode(left, right);
}

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

template<class Iter,
         class = typename std::enable_if<
                 std::is_convertible<
                         typename std::iterator_traits<Iter>::iterator_category,
                         std::input_iterator_tag>::value>::type>
std::unique_ptr<ASTNode> make_all(Iter begin, Iter end)
{
    return std::unique_ptr<ASTNode>(create_regexes(begin, end));
}

std::unique_ptr<ASTNode> make_all(std::initializer_list<const char*> list)
{
    return make_all(list.begin(), list.end());
}

void test(const TransitionTable& table, const std::string& s)
{
    int state = 0;
    for (char ch : s) {
        int next_state = table[state][ch];
        if (next_state == 1) {
            break;
        }
        state = next_state;
        std::cout << state << ' ' << ch << '\n';
    }
    auto it = table.find_final(state);
    if (it != table.final_states_end()) {
        std::cout << "parsed a(n) " << it->second << '\n';
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
    auto regex = make_all(
            {"auto", "break", "case", "char", "const", "continue", "default",
             "do", "double", "else", "enum", "extern", "float", "for", "goto",
             "if", "int", "long", "register", "return", "short", "signed",
             "sizeof", "static", "struct", "switch", "typedef", "union",
             "unsigned",
             "void", "volatile", "while",
             "[A-Za-z_][A-Za-z0-9_]*", "[1-9][0-9]*"
            });
    TreeFunctions calc(regex.get());
    auto dtrans = make_transition_table(calc);
    std::cout << dtrans.size() << '\n';

    test(dtrans, "if ", "then ", "else ", "HelloWorld");

    return 0;
}
