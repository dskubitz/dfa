#include <gtest/gtest.h>

#include <Parser.h>
#include <TransitionTable.h>
#include <DerivativeClass.h>

TEST(TransitionTableTests, TransitionTableTest)
{
    Parser parser;
    std::vector<std::string> regexps = {
            "and", "class", "else", "false", "fun",
            "for", "if", "nil", "or", "print", "return",
            "super", "this", "true", "var", "while",
            "[A-Za-z_][A-Za-z_0-9]*", "[0-9]+(\\.[0-9]+)?",
            "\"(\\.|[^\\\"\n])*\"", "[ \t\v\n\f]",
    };

    std::vector<Regexp> regular_vector;
    for (auto& i : regexps)
        regular_vector.push_back(parser.parse(i));

    auto table = make_transition_table(regular_vector);

}
