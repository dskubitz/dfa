#include <gtest/gtest.h>
#include <lexer/Parser.h>

class ParserTests : public ::testing::Test {
protected:
    Parser parser_;
    std::ostringstream out;
};
TEST_F(ParserTests, SingleCharactersBad)
{
    EXPECT_THROW(parser_.parse("*"), ParserError);
    EXPECT_THROW(parser_.parse("["), ParserError);
    EXPECT_THROW(parser_.parse("]"), ParserError);
    EXPECT_THROW(parser_.parse("|"), ParserError);
    EXPECT_THROW(parser_.parse("+"), ParserError);
    EXPECT_THROW(parser_.parse("?"), ParserError);
    EXPECT_THROW(parser_.parse("("), ParserError);
    EXPECT_THROW(parser_.parse(")"), ParserError);
}

TEST_F(ParserTests, EmptyCharacterClass)
{
    EXPECT_THROW(parser_.parse("[]"), ParserError);
    EXPECT_THROW(parser_.parse("[^]"), ParserError);
}

TEST_F(ParserTests, UnmatchedParenthesisOrBracket)
{
    EXPECT_THROW(parser_.parse("(abcd"), ParserError);
    EXPECT_THROW(parser_.parse("[abcd"), ParserError);
    EXPECT_THROW(parser_.parse("abcd)"), ParserError);
    EXPECT_THROW(parser_.parse("abcd]"), ParserError);
}

TEST_F(ParserTests, TrailingEscape)
{
    EXPECT_THROW(parser_.parse("\\"), ParserError);

}

TEST_F(ParserTests, InvalidRange)
{
    EXPECT_THROW(parser_.parse("[b-a]"), ParserError);
}

TEST_F(ParserTests, MinusInCharacterClass)
{
    auto r1 = parser_.parse("[-a-c]");
    auto r2 = parser_.parse("[a-c-]");
    EXPECT_EQ(r1, r2);
}

TEST_F(ParserTests, Intersection)
{
    auto r1 = parser_.parse("ab&cd");
    EXPECT_EQ(r1->to_string(), "((a.b)&(c.d))");
}

TEST_F(ParserTests, Complement)
{
    auto r = parser_.parse("~(abc)");
    EXPECT_EQ(r->to_string(), "~(((a.b).c))");
    auto r2 = parser_.parse("~abc");
    EXPECT_EQ(r2->to_string(), "((~(a).b).c)");
}
