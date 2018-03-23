#include <gtest/gtest.h>
#include <Parser.h>
#include <Regex.h>
#include <TransitionTable.h>
#include <Derivative.h>
#include <DerivativeClass.h>

TEST(TransitionTableTests, TransitionTableTest)
{
    auto re = Parser{}.parse("def|[abc][abc_01]*");
    auto table = make_transition_table(re);
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
    Derivative D;
    DerivativeClass derivativeClass;
    auto cl = derivativeClass.evaluate(re);
    for (auto& set : cl) {
        std::cout << set.to_string() << '\n';
    }
    auto r1 = D.derive(re, 'K');
    auto r2 = D.derive(re, 'b');
    auto r3 = D.derive(re, 'd');
    std::cout << r1.get()->to_string() << '\n';
    std::cout << r2.get()->to_string() << '\n';
    std::cout << r3.get()->to_string() << '\n';
}
