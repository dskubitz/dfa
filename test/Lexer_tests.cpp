#include <gtest/gtest.h>
#include <Lexer.h>
#include <Parser.h>
#include <TransitionTable.h>

class LexerTests : public ::testing::Test {
protected:
    std::unique_ptr<ASTNode> regex;
    std::stringstream input;
    std::unique_ptr<TransitionTable> table;
    std::unique_ptr<Lexer> lexer;

    void SetUp() override
    {
        regex = Parser{}.parse(
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
        table = std::make_unique<TransitionTable>(
                make_transition_table(regex.get()));
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
    input << "helloWorld1234 1234helloWorld\nhello_world__123__\n1.2345";
    std::vector<std::pair<std::string, std::string>> expects{
            {"helloWorld1234",     "identifier"},
            {" ",                  "space"},
            {"1234",               "number"},
            {"helloWorld",         "identifier"},
            {"\n",                 "new_line"},
            {"hello_world__123__", "identifier"},
            {"\n",                 "new_line"},
            {"1.2345",             "float"},
    };
    auto& stoi_table = lexer->stoitoken_kinds();
    for (auto& expect : expects) {
        Token tok = lexer->scan();
        EXPECT_EQ(tok.lexeme, expect.first);
        EXPECT_EQ(tok.kind, stoi_table.at(expect.second));
    }
}

class OptionalRegexTests : public ::testing::Test {
protected:
    std::stringstream input;
    std::unique_ptr<ASTNode> re;
    std::unique_ptr<TransitionTable> tbl;
    std::unique_ptr<Lexer> lex;

    void SetUp() override
    {
        re = Parser{}.parse(
                {
                        {"colou?r", "1"},
                        {" ",       "space"},
                        {".",       "error"},
                }
        );
        tbl = std::make_unique<TransitionTable>(
                make_transition_table(re.get()));
        lex = std::make_unique<Lexer>(*tbl, input);
    }
};

TEST_F(OptionalRegexTests, test)
{
    input << "colour color";
    auto tok = lex->scan();
    EXPECT_EQ(tok.lexeme, "colour");
    tok = lex->scan();
    tok = lex->scan();
    EXPECT_EQ(tok.lexeme, "color");
}

class NumberTests : public ::testing::Test {
protected:
    std::stringstream input;
    std::unique_ptr<ASTNode> re;
    std::unique_ptr<TransitionTable> tbl;
    std::unique_ptr<Lexer> lex;

    void SetUp() override
    {
        re = Parser{}.parse(
                {
                        {"([0-9]+(\\.)?([0-9]+))", "float"},
                        {".",                      "error"},
                }
        );
        tbl = std::make_unique<TransitionTable>(
                make_transition_table(re.get()));
        lex = std::make_unique<Lexer>(*tbl, input);
    }
};

TEST_F(NumberTests, TestName)
{
    input << " 10.19 1";
    while (lex->good()) {
        auto tok = lex->scan();
        std::cout << tok.lexeme << ' ' << lex->itostoken_kinds().at(tok.kind)
                  << '\n';
    }

}
