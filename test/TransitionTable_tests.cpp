#include <gtest/gtest.h>
#include <Parser.h>
#include <TransitionTable.h>
#include <DerivativeClass.h>

TEST(TransitionTableTests, TransitionTableTest)
{
    auto re = Parser{}.parse("[A-Za-z_][A-Za-z_0-9]*");
    auto re2 = Parser{}.parse("[0-9]+(\\.[0-9]+)?");

    std::vector<Regex> regular_vector{re, re2};
    auto table = make_transition_table(regular_vector);
    /*
    int n = 0;
    for (auto& state : table) {
        std::cout << "state " << n << '\n';
        for (int i = 0; i < 128; ++i) {
            if (!(isgraph(i) || isspace(i)))
                continue;
            if (isgraph(i))
                std::cout << "q(" << char(i) << ") = " << state[i] << ' ';
            else
                std::cout << "q(" << i << ") = " << state[i] << ' ';
            if (i % 8 == 0)
                std::cout << '\n';
        }
        std::cout << '\n';
        n++;
    }
    */
    int state = 0;
    for (auto c : "123.01234 abcd") {
        state = table[state][c];
        std::cout << state << ' ' << c << '\n';
    }
}
