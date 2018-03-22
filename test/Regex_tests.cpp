#include <gtest/gtest.h>
#include <Regex.h>
#include <Parser.h>
#include <unused/PrettyPrinter.h>

class RegexTests : public ::testing::Test {
protected:
    Parser parser;

};
TEST_F(RegexTests, Cat)
{
    std::unique_ptr<Regex> re(
            make_cat(new Empty, new Symbol('a')));
    auto p = dynamic_cast<Empty*>(re.get());
    ASSERT_NE(p, nullptr);
    re.reset(make_cat(new Epsilon, new Symbol('a')));
    auto q = dynamic_cast<Symbol*>(re.get());
    ASSERT_NE(q, nullptr);
    EXPECT_EQ(q->value(), 'a');
}

TEST_F(RegexTests, Union)
{
    std::unique_ptr<Regex> re(
            make_union(new Empty, new Symbol('a')));
    auto p = dynamic_cast<Symbol*>(re.get());
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->value(), 'a');
    re.reset(make_union(new Symbol('a'), new Symbol('a')));
    p = dynamic_cast<Symbol*>(re.get());
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->value(), 'a');
}

TEST_F(RegexTests, Star)
{
    std::unique_ptr<Regex> re(
            make_star(new Closure(new Closure(new Symbol('a')))));
    auto p = dynamic_cast<Closure*>(re.get());
    ASSERT_NE(p, nullptr);
    auto q = dynamic_cast<const Symbol*>(p->expr());
    ASSERT_NE(q, nullptr);
}

TEST_F(RegexTests, Intersection)
{
    std::unique_ptr<Regex> re(
            make_intersection(new Empty, new Symbol('a')));
    auto p = dynamic_cast<Empty*>(re.get());
    ASSERT_NE(p, nullptr);
    re.reset(make_intersection(new Symbol('a'), new Symbol('a')));
    auto p2 = dynamic_cast<Symbol*>(re.get());
    ASSERT_NE(p2, nullptr);
    EXPECT_EQ(p2->value(), 'a');
}

TEST_F(RegexTests, Complement)
{
    std::unique_ptr<Regex> re(
            make_complement(new Complement(new Symbol('a'))));
    auto p = dynamic_cast<Symbol*>(re.get());
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->value(), 'a');
}

TEST_F(RegexTests, Test)
{
    auto re1 = parser.parse("");
    auto re2 = parser.parse("a");
    auto re3 = parser.parse("");
    EXPECT_FALSE(*re1 == *re2);

    EXPECT_EQ(*re1, *re3);

    auto re4 = parser.parse("(a|b)*abb");
    auto re5 = parser.parse("(ab*|cd*)");
    EXPECT_NE(*re4, *re5);

    auto re6 = parser.parse("(a|b)*abb");
    EXPECT_TRUE(*re4 == *re6);
}

TEST_F(RegexTests, ToString)
{
    auto re = parser.parse("[A-Z_a-z][0-9A-Z_a-z]*");
    EXPECT_EQ(re->to_string(),
              "((((((((((((((((((((((((((((((((((((("
                      "((((((((((((((((A|B)|C)|D)|E)|F"
                      ")|G)|H)|I)|J)|K)|L)|M)|N)|O)|P)|Q)|R)|S)|T)|U"
                      ")|V)|W)|X)|Y)|Z)|_)|a)|b)|c)|d)|e)|f)|g)|"
                      "h)|i)|j)|k)|l)|m)|n)|o)|p)|q)|r)|s)|t)|u)|v)|w)|x)|y)|z)"
                      ".((((((((((((((((((((((((((((((((((((((((((((((((("
                      "((((((((((((((0|1)|2)|3)|4)|5)|6)|7)|8)|"
                      "9)|A)|B)|C)|D)|E)|F)|G)|H)|I)|J)|K)|L)|M)"
                      "|N)|O)|P)|Q)|R)|S)|T)|U)|V)|W)|X)|Y)|Z)|_)|"
                      "a)|b)|c)|d)|e)|f)|g)|h)|i)|j)|k)|l)|m)|n)|o)|p)|"
                      "q)|r)|s)|t)|u)|v)|w)|x)|y)|z)*))");

}
