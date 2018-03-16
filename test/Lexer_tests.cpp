#include <gtest/gtest.h>
#include <Lexer.h>
#include <Parser.h>
#include <TransitionTable.h>

namespace {
std::unique_ptr<ASTNode> regex = Parser{}.parse(
        {
                {"\n",                     "new_line"},
                {"[ \t\v\f]+",             "space"},
                {".",                      "error"},
                {"[0-9]+",                 "number"},
                {"[0-9]+\\.[0-9]+",        "float"},
                {"\\\"[^\\\"]*\"",         "string"},
                {"[A-Za-z_][A-Za-z0-9_]*", "identifier"},
        }
);

std::unique_ptr<TransitionTable> table = std::make_unique<TransitionTable>(
        make_transition_table(regex.get()));
}
class LexerTests : public ::testing::Test {
protected:
    std::stringstream input;
    std::unique_ptr<Lexer> lexer;

    void SetUp() override
    {
        lexer = std::make_unique<Lexer>(*table, input);
    }
};

TEST_F(LexerTests, ScanNumber)
{
    input << "1234";
    Token tok = lexer->scan();
    Token tst = Token{"1234", lexer->stoitoken_kinds().at("number"), 1, 1};
    EXPECT_EQ(tok.lexeme, tst.lexeme);
    EXPECT_EQ(tok.kind, tst.kind);
    EXPECT_EQ(tok.line, tst.line);
    EXPECT_EQ(tok.column, tst.column);
    EXPECT_EQ(tok, tst);
}

TEST_F(LexerTests, ScanMultiple)
{
    input << "helloWorld1234 1234helloWorld\n1234.5678";
    std::vector<std::pair<std::string, std::string>> expects {
            {"helloWorld1234", "identifier"},
            {" ", "space"},
            {"1234", "number"},
            {"helloWorld", "identifier"},
            {"\n", "new_line"},
            {"1234.5678", "float"},
    };
    auto& stoi_table = lexer->stoitoken_kinds();
    for (auto& expect : expects) {
        Token tok = lexer->scan();
        EXPECT_EQ(tok.lexeme, expect.first);
        EXPECT_EQ(tok.kind, stoi_table.at(expect.second));
    }

}
