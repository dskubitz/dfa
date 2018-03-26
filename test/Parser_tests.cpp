#include <gtest/gtest.h>
#include <Parser.h>

class ParserTests : public ::testing::Test {
protected:
    Parser parser;
    std::ostringstream out;
};

TEST_F(ParserTests, SingleCharactersBad)
{
    EXPECT_THROW(parser.parse("*"), ParserError);
    EXPECT_THROW(parser.parse("["), ParserError);
    EXPECT_THROW(parser.parse("]"), ParserError);
    EXPECT_THROW(parser.parse("|"), ParserError);
    EXPECT_THROW(parser.parse("+"), ParserError);
    EXPECT_THROW(parser.parse("?"), ParserError);
    EXPECT_THROW(parser.parse("("), ParserError);
    EXPECT_THROW(parser.parse(")"), ParserError);
}

TEST_F(ParserTests, EmptyCharacterClass)
{
    EXPECT_THROW(parser.parse("[]"), ParserError);
    EXPECT_THROW(parser.parse("[^]"), ParserError);
}

TEST_F(ParserTests, UnmatchedParenthesisOrBracket)
{
    EXPECT_THROW(parser.parse("(abcd"), ParserError);
    EXPECT_THROW(parser.parse("[abcd"), ParserError);
    EXPECT_THROW(parser.parse("abcd)"), ParserError);
    EXPECT_THROW(parser.parse("abcd]"), ParserError);
}

TEST_F(ParserTests, TrailingEscape)
{
    EXPECT_THROW(parser.parse("\\"), ParserError);

}

TEST_F(ParserTests, InvalidRange)
{
    EXPECT_THROW(parser.parse("[b-a]"), ParserError);
}

TEST_F(ParserTests, MinusInCharacterClass)
{
    auto r1 = parser.parse("[-a-c]");
    auto r2 = parser.parse("[a-c-]");
    EXPECT_EQ(r1, r2);
}

TEST_F(ParserTests, Intersection)
{
    auto r1 = parser.parse("ab&cd");
    EXPECT_EQ(r1->to_string(), "((a.b)&(c.d))");
}
