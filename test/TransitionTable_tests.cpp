#include <gtest/gtest.h>

#include <Parser.h>
#include <TransitionFunction.h>
#include <Nullable.h>

TEST(TransitionTableTests, TransitionTableTest)
{
    Parser parser;
    Nullable nullable;
    std::vector<std::string> regexps = {
            "and",
            "class",
            "else",
            "false",
            "fun",
            "for",
            "if",
            "nil",
            "or",
            "print",
            "return",
            "super",
            "this",
            "true",
            "var",
            "while",
            "[A-Za-z_][A-Za-z_0-9]*",
            "[0-9]+(\\.[0-9]+)?",
            "\"(\\.|[^\\\"\n])*\"",
            "[ \t\v\n\f]",
    };

    std::vector<Regexp> regular_vector;
    for (auto& i : regexps)
        regular_vector.push_back(parser.parse(i));

    auto&&[table, states] = make_DFA(regular_vector);
    std::cout << typeid(table).name() << '\n';
    std::cout << typeid(states).name() << '\n';
    for (auto&&[state, index] : states) {
        int snum = 0;
        for (auto& i : state){
            if (nullable.evaluate(i)) {
                std::cout << index << " accepts " << regexps.at(snum) << '\n';
                break;
            }
            snum++;
        }
    }
}
