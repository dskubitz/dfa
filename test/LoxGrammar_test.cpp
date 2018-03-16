#include <gtest/gtest.h>
#include <Parser.h>
#include <TransitionTable.h>
#include <TreeFunctions.h>

std::string simulate(const TransitionTable& table, const std::string& s)
{
    int state = 0;
    for (char ch : s) {
        int next_state = table[state][ch];
        if (next_state == 1) {
            break;
        }
        state = next_state;
    }

    auto it = table.final_states().find(state);
    if (it != table.final_states().end())
        return it->second;

    return {};
}

class LoxGrammarTests : public ::testing::Test {
protected:
    std::unique_ptr<ASTNode> regex;

    void SetUp() override
    {
        regex = Parser{}.parse(
                {
                        {"and",                    "and"},
                        {"class",                  "class"},
                        {"else",                   "else"},
                        {"false",                  "false"},
                        {"for",                    "for"},
                        {"fun",                    "fun"},
                        {"if",                     "if"},
                        {"nil",                    "nil"},
                        {"or",                     "or"},
                        {"print",                  "print"},
                        {"return",                 "return"},
                        {"super",                  "super"},
                        {"this",                   "this"},
                        {"true",                   "true"},
                        {"var",                    "var"},
                        {"while",                  "while"},
                        {"[0-9]+",                 "number"},
                        {"[0-9]+\\.[0-9]+",        "float"},
                        {"\\\"[^\\\"]*\"",         "string"},
                        {"[A-Za-z_][A-Za-z0-9_]*", "identifier"},
                        {"==",                     "equal_equal"},
                        {"!=",                     "bang_equal"},
                        {"<=",                     "less_equal"},
                        {">=",                     "greater_equal"},
                        {"\\<",                    "less"},
                        {"\\>",                    "greater"},
                        {"\\+",                    "plus"},
                        {"\\-",                    "minus"},
                        {"\\*",                    "star"},
                        {"\\/",                    "slash"},
                        {"\\=",                    "equal"},
                        {"\\.",                    "dot"},
                        {"\n",                     "new_line"},
                        {"[ \t\v\f]+",             "space"},
                        {".",                      "error"},
                }
        );
    }
};

TEST_F(LoxGrammarTests, Test)
{
    TreeFunctions functions(regex.get());
    TransitionTable table(functions);
    EXPECT_EQ(simulate(table, "and"), "and");
    EXPECT_EQ(simulate(table, "class"), "class");
    EXPECT_EQ(simulate(table, "else"), "else");
    EXPECT_EQ(simulate(table, "false"), "false");
    EXPECT_EQ(simulate(table, "for"), "for");
    EXPECT_EQ(simulate(table, "fun"), "fun");
    EXPECT_EQ(simulate(table, "if"), "if");
    EXPECT_EQ(simulate(table, "nil"), "nil");
    EXPECT_EQ(simulate(table, "or"), "or");
    EXPECT_EQ(simulate(table, "print"), "print");
    EXPECT_EQ(simulate(table, "return"), "return");
    EXPECT_EQ(simulate(table, "super"), "super");
    EXPECT_EQ(simulate(table, "this"), "this");
    EXPECT_EQ(simulate(table, "var"), "var");
    EXPECT_EQ(simulate(table, "while"), "while");
    EXPECT_EQ(simulate(table, "42"), "number");
    EXPECT_EQ(simulate(table, "0"), "number");
    EXPECT_EQ(simulate(table, "09"), "number");
    EXPECT_EQ(simulate(table, "0.0"), "float");
    EXPECT_EQ(simulate(table, "3.14159"), "float");
    EXPECT_EQ(simulate(table, "Hello"), "identifier");
    EXPECT_EQ(simulate(table, "=="), "equal_equal");
    EXPECT_EQ(simulate(table, "!="), "bang_equal");
    EXPECT_EQ(simulate(table, "<="), "less_equal");
    EXPECT_EQ(simulate(table, ">="), "greater_equal");
    EXPECT_EQ(simulate(table, ">"), "greater");
    EXPECT_EQ(simulate(table, "<"), "less");
    EXPECT_EQ(simulate(table, "+"), "plus");
    EXPECT_EQ(simulate(table, "-"), "minus");
    EXPECT_EQ(simulate(table, "*"), "star");
    EXPECT_EQ(simulate(table, "/"), "slash");
    EXPECT_EQ(simulate(table, "="), "equal");
    EXPECT_EQ(simulate(table, "."), "dot");
    EXPECT_EQ(simulate(table, "\n"), "new_line");
    EXPECT_EQ(simulate(table, " \v\t\f"), "space");
    EXPECT_EQ(simulate(table, "|"), "error");
}
