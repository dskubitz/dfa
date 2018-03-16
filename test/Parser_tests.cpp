#include <gtest/gtest.h>
#include <Parser.h>
#include <PrettyPrinter.h>

class ParserTests : public ::testing::Test {
protected:
    Parser parser;
    std::ostringstream out;
    PrettyPrinter printer {out};
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

TEST_F(ParserTests, CharacterClassTwoChars)
{
    auto regex1 = parser.parse("[ab]");
    printer.print(regex1.get());
    EXPECT_EQ(out.str(), "(cat (union a b) #)\n");
}

TEST_F(ParserTests, CharacterClassTwoCharRange)
{
    auto regex1 = parser.parse("[a-b]");
    printer.print(regex1.get());
    EXPECT_EQ(out.str(), "(cat (union a b) #)\n");
}

TEST_F(ParserTests, CharacterClassTwoCharRangePlusThird)
{
    auto regex1 = parser.parse("[a-be]");
    printer.print(regex1.get());
    EXPECT_EQ(out.str(), "(cat (union (union a b) e) #)\n");
}

TEST_F(ParserTests, NegatedCharacterClassRange)
{
    auto regex1 = parser.parse("[^A-Za-z0-9!-/:-~\n]");
    printer.print(regex1.get());
    EXPECT_EQ(out.str(),
              "(cat (union (union (union (union \t \v) \f) \r)  ) #)\n");
}

TEST_F(ParserTests, SingleCharactersGood)
{
    auto regex1 = parser.parse("a");
    printer.print(regex1.get());
    EXPECT_EQ(out.str(), "(cat a #)\n");
}

TEST_F(ParserTests, MultipleStars)
{
    EXPECT_THROW(parser.parse("a**"), ParserError);
    EXPECT_THROW(parser.parse("a?*"), ParserError);
    EXPECT_THROW(parser.parse("a*?"), ParserError);
    EXPECT_THROW(parser.parse("a+*"), ParserError);
    EXPECT_THROW(parser.parse("a*+"), ParserError);
    auto regex = parser.parse("(a*)*");
    printer.print(regex.get());
    EXPECT_EQ(out.str(), "(cat (star (star a)) #)\n");
}

TEST_F(ParserTests, Precedence)
{
    auto regex1 = parser.parse("((ab*)c)|(def*)");
    auto regex2 = parser.parse("ab*c|def*");
    printer.print(regex1.get());
    std::string str1 = out.str();
    out = std::ostringstream{};
    printer.print(regex2.get());
    std::string str2 = out.str();
    EXPECT_EQ(str1, str2);
}
