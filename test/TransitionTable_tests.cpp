#include <gtest/gtest.h>

#include <Parser.h>
#include <TransitionTable.h>
#include <Nullable.h>
#include <cstdint>

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
            "\\+",
            "\\-",
            "\\*",
            "\\/",
            "==",
            "\\=",
            "!=",
            "\\!",
            "<=",
            "\\<",
            ">=",
            "\\>",
            "[ \t\v\n\f]",
    };

    std::vector<Regexp> regular_vector;
    for (auto& i : regexps)
        regular_vector.push_back(parser.parse(i));

    DFA dfa = make_DFA(regular_vector);
    std::cout << typeid(dfa).name() << '\n';
    for (auto& i : dfa.table) {
        for (auto& j : i) {
            std::cout << j << ' ';
        }
        std::cout << "\n\n";
    }
    for (auto& i : dfa.accept_map) {
        std::cout << i.first << " -> " << i.second << '\n';
    }
    std::cout << "dead -> " << dfa.dead_state << '\n';
    for (auto& i : dfa.state_map) {
        std::cout << i.second << " -> ";
        std::copy(i.first.begin(), i.first.end(),
                  std::ostream_iterator<Regexp>(std::cout));
        auto it = dfa.accept_map.find(i.second);
        if (it != dfa.accept_map.end())
            std::cout << ' ' << regexps.at(dfa.accept_map.at(i.second));
        std::cout << '\n';
    }
}
