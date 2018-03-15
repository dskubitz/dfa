#include <iostream>

#include "ASTNode.h"
#include "TransitionTable.h"
#include "Parser.h"
#include "TreeFunctions.h"

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
    auto it = table.final_states().find(state);
    if (it != table.final_states().end()) {
        std::cout << it->second << '\n';
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
    std::unique_ptr<ASTNode> regex(Parser{}.parse(
            {
                    {"if",                     "if"},
                    {"then",                   "then"},
                    {"else",                   "else"},
                    {"[A-Za-z_][A-Za-z0-9_]*", "identifier"}}));
    TreeFunctions calc(regex.get());
    TransitionTable dtrans(calc);
    std::cout << dtrans.size() << '\n';

    test(dtrans, "a", "if", "then", "else", "HelloWorld");

    /*
    std::cout << "int dtrans[" << dtrans.size() << "][128] = {\n";
    for (auto& state:dtrans){
        std::cout << "\n\t{";
        for (int i = 0; i < 128; ) {
            std::cout << state[i++];
            if (i == 128) {
                break;
            }
            std::cout << ", ";
            if (i % 16 == 0)
                std::cout << "\n\t";
        }
        std::cout << "},\n";
    }
    std::cout << "};";
    */
    return 0;
}
