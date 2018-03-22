#include <gtest/gtest.h>
#include <Parser.h>
#include <Regex.h>
#include <TransitionTable.h>
#include <Derivative.h>

TEST(TransitionTableTests, TransitionTableTest)
{
    auto re = Parser{}.parse("a+b*");
    auto table = make_transition_table(re);
    int i = 0;
    for (auto& st : table) {
        for (auto& trs : st) {
            std::cout << trs << ' ';
            if (++i % 64 == 0)
                std::cout << '\n';

        }
        std::cout << '\n';
    }
}
