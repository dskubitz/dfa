#include <gtest/gtest.h>

#include <Regexp.h>
#include <Parser.h>
#include <unordered_set>

class RegexTests : public ::testing::Test {
protected:
    Parser parser;
};

TEST_F(RegexTests, Cat)
{
    std::unique_ptr<RegexNode> re(
            make_cat(new Empty, new Symbol('a')));
    auto p = dynamic_cast<Empty*>(re.get());
    ASSERT_NE(p, nullptr);
    re.reset(make_cat(new Epsilon, new Symbol('a')));
    auto q = dynamic_cast<Symbol*>(re.get());
    ASSERT_NE(q, nullptr);
    EXPECT_TRUE(q->values().test('a'));
}

TEST_F(RegexTests, Union)
{
    std::unique_ptr<RegexNode> re(
            make_union(new Empty, new Symbol('a')));
    auto p = dynamic_cast<Symbol*>(re.get());
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(p->values().test('a'));
    re.reset(make_union(new Symbol('a'), new Symbol('a')));
    p = dynamic_cast<Symbol*>(re.get());
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(p->values().test('a'));
}

TEST_F(RegexTests, Star)
{
    std::unique_ptr<RegexNode> re(
            make_star(new Closure(new Closure(new Symbol('a')))));
    auto p = dynamic_cast<Closure*>(re.get());
    ASSERT_NE(p, nullptr);
    auto q = dynamic_cast<const Symbol*>(p->expr());
    ASSERT_NE(q, nullptr);
}

TEST_F(RegexTests, Intersection)
{
    std::unique_ptr<RegexNode> re(
            make_intersection(new Empty, new Symbol('a')));
    auto p = dynamic_cast<Empty*>(re.get());
    ASSERT_NE(p, nullptr);
    re.reset(make_intersection(new Symbol('a'), new Symbol('a')));
    auto p2 = dynamic_cast<Symbol*>(re.get());
    ASSERT_NE(p2, nullptr);
    EXPECT_TRUE(p2->values().test('a'));
}

TEST_F(RegexTests, Complement)
{
    std::unique_ptr<RegexNode> re(
            make_complement(new Complement(new Symbol('a'))));
    auto p = dynamic_cast<Symbol*>(re.get());
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(p->values().test('a'));
}

TEST_F(RegexTests, Test)
{
    auto re1 = parser.parse("");
    auto re2 = parser.parse("a");
    auto re3 = parser.parse("");
    EXPECT_FALSE(re1 == re2);

    EXPECT_EQ(re1, re3);

    auto re4 = parser.parse("(a|b)*abb");
    auto re5 = parser.parse("(ab*|cd*)");
    EXPECT_NE(re4, re5);

    auto re6 = parser.parse("(a|b)*abb");
    EXPECT_TRUE(re4 == re6);
}

TEST_F(RegexTests, WrapperClass)
{
    Regexp regex(parser.parse("(a|b)*abb"));
    Regexp re2 = regex;
    EXPECT_EQ(regex, re2);
    Regexp re3 = std::move(re2);
    re2 = Regexp(parser.parse("(ab*|cd*)"));
    EXPECT_NE(regex, re2);
    EXPECT_EQ(regex, re3);
}

TEST_F(RegexTests, Hash)
{
    std::set<Regexp> regexes;
    auto && [it, inserted] = regexes.insert(parser.parse("(a|b)*abb"));
    EXPECT_TRUE(inserted);
    std::tie(it, inserted) = regexes.insert(parser.parse("(a|b)*abb"));
    EXPECT_FALSE(inserted);
    auto iter = regexes.find(*it);
    EXPECT_NE(iter, regexes.end());
}

TEST_F(RegexTests, StrictWeakOrder)
{
    Regexp re0 = parser.parse(".");
    Regexp re1 = parser.parse("[aeiou]");
    Regexp re2 = parser.parse("[0-9]");
    Regexp re3 = parser.parse("[aeiou]");
    Regexp re4 = parser.parse("[A-Z_a-z_0-9]");
    // for all a, comp(a,a)==false
    EXPECT_FALSE(re0 < re0);
    EXPECT_FALSE(re1 < re1);
    EXPECT_FALSE(re2 < re2);
    EXPECT_FALSE(re3 < re3);
    EXPECT_FALSE(re4 < re4);
    // if comp(a,b)==true then comp(b,a)==false
    EXPECT_TRUE((re0 < re1) == !(re1 < re0));
    EXPECT_TRUE((re2 < re3) == !(re3 < re2));
    // if comp(a,b) and comp(b,c) then comp(a,c)
    EXPECT_TRUE(((re0 < re1) && (re2 < re3)) == (re0 < re3));
}

TEST_F(RegexTests, SingletonEmpty)
{
    auto empty1 = new Empty;
    auto empty2 = new Empty;
    EXPECT_EQ(empty1, empty2);
}

TEST_F(RegexTests, Swap)
{
    Regexp re1 = parser.parse("[aeiou]");
    Regexp re2 = parser.parse("[A-Z_a-z_0-9]");
    Regexp re3 = parser.parse("[aeiou]");
    Regexp re4 = parser.parse("[A-Z_a-z_0-9]");
    using std::swap;
    swap(re1, re2);
    EXPECT_EQ(re2, re3);
    EXPECT_EQ(re1, re4);
}
