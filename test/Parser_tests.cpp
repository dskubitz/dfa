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
