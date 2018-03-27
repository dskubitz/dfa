#include <gtest/gtest.h>

#include <Parser.h>
#include <TransitionFunction.h>
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
            "[ \t\v\n\f]",
    };

    std::vector<Regexp> regular_vector;
    for (auto& i : regexps)
        regular_vector.push_back(parser.parse(i));

    TransitionFunction table;
    StateMap states;
    std::tie(table, states) = make_DFA(regular_vector);

    for (int i = 0; i < table.size(); ++i) {
        std::cout << i << '\n';
        for (int j = 0; j < 128;) {
            std::cout << table[i][j++] << ' ';
            if (j % 16 == 0)
                std::cout << '\n';
        }
        std::cout << '\n';
    }
    for (auto& state : states) {
        std::cout << state.second << ' ';
        if (std::all_of(state.first.begin(), state.first.end(),
                        [](const Regexp& a) {
                            return a->type() == RegexNode::Type::Empty;
                        })) {
            std::cout << "dead\n";
        }
        for (auto& i : state.first) {
            std::cout << i->to_string() << ' ';
        }
        std::cout << '\n';
    }
    std::cout << sizeof(intmax_t) << '\n';
}
