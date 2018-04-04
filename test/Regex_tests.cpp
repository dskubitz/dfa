#include <gtest/gtest.h>

#include <unordered_set>
#include <lexer/Parser.h>
#include <lexer/Regexp.h>

class RegexTests : public ::testing::Test {
protected:
    Parser parser_;
};

TEST_F(RegexTests, Cat)
{
    std::unique_ptr<Regex::Node> re(
            make_concatenation(new Regex::Empty, new Regex::Symbol('a')));
    auto p = dynamic_cast<Regex::Empty*>(re.get());
    ASSERT_NE(p, nullptr);
    re.reset(make_concatenation(new Regex::Epsilon, new Regex::Symbol('a')));
    auto q = dynamic_cast<Regex::Symbol*>(re.get());
    ASSERT_NE(q, nullptr);
    EXPECT_TRUE(q->values().test('a'));
}

TEST_F(RegexTests, Union)
{
    std::unique_ptr<Regex::Node> re(
            make_union(new Regex::Empty, new Regex::Symbol('a')));
    auto p = dynamic_cast<Regex::Symbol*>(re.get());
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(p->values().test('a'));
    re.reset(make_union(new Regex::Symbol('a'), new Regex::Symbol('a')));
    p = dynamic_cast<Regex::Symbol*>(re.get());
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(p->values().test('a'));
}

TEST_F(RegexTests, Star)
{
    std::unique_ptr<Regex::Node> re(
            make_closure(new Regex::Closure(
                    new Regex::Closure(new Regex::Symbol('a')))));
    auto p = dynamic_cast<Regex::Closure*>(re.get());
    ASSERT_NE(p, nullptr);
    auto q = dynamic_cast<const Regex::Symbol*>(p->expr());
    ASSERT_NE(q, nullptr);
}

TEST_F(RegexTests, Intersection)
{
    std::unique_ptr<Regex::Node> re(
            make_intersection(new Regex::Empty, new Regex::Symbol('a')));
    auto p = dynamic_cast<Regex::Empty*>(re.get());
    ASSERT_NE(p, nullptr);
    re.reset(make_intersection(new Regex::Symbol('a'), new Regex::Symbol('a')));
    auto p2 = dynamic_cast<Regex::Symbol*>(re.get());
    ASSERT_NE(p2, nullptr);
    EXPECT_TRUE(p2->values().test('a'));
}

TEST_F(RegexTests, Complement)
{
    std::unique_ptr<Regex::Node> re(
            make_complement(new Regex::Complement(new Regex::Symbol('a'))));
    auto p = dynamic_cast<Regex::Symbol*>(re.get());
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(p->values().test('a'));
}

TEST_F(RegexTests, Test)
{
    auto re1 = parser_.parse("");
    auto re2 = parser_.parse("a");
    auto re3 = parser_.parse("");
    EXPECT_FALSE(re1 == re2);

    EXPECT_EQ(re1, re3);

    auto re4 = parser_.parse("(a|b)*abb");
    auto re5 = parser_.parse("(ab*|cd*)");
    EXPECT_NE(re4, re5);

    auto re6 = parser_.parse("(a|b)*abb");
    EXPECT_TRUE(re4 == re6);
}

TEST_F(RegexTests, WrapperClass)
{
    Regexp regex(parser_.parse("(a|b)*abb"));
    Regexp re2 = regex;
    EXPECT_EQ(regex, re2);
    Regexp re3 = std::move(re2);
    re2 = Regexp(parser_.parse("(ab*|cd*)"));
    EXPECT_NE(regex, re2);
    EXPECT_EQ(regex, re3);
}

TEST_F(RegexTests, Hash)
{
    std::set<Regexp> regexes;
    auto && [it, inserted] = regexes.insert(parser_.parse("(a|b)*abb"));
    EXPECT_TRUE(inserted);
    std::tie(it, inserted) = regexes.insert(parser_.parse("(a|b)*abb"));
    EXPECT_FALSE(inserted);
    auto iter = regexes.find(*it);
    EXPECT_NE(iter, regexes.end());
}

TEST_F(RegexTests, StrictWeakOrder)
{
    Regexp re0 = parser_.parse(".");
    Regexp re1 = parser_.parse("[aeiou]");
    Regexp re2 = parser_.parse("[0-9]");
    Regexp re3 = parser_.parse("[aeiou]");
    Regexp re4 = parser_.parse("[A-Z_a-z_0-9]");
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

TEST_F(RegexTests, Swap)
{
    Regexp re1 = parser_.parse("[aeiou]");
    Regexp re2 = parser_.parse("[A-Z_a-z_0-9]");
    Regexp re3 = parser_.parse("[aeiou]");
    Regexp re4 = parser_.parse("[A-Z_a-z_0-9]");
    using std::swap;
    swap(re1, re2);
    EXPECT_EQ(re2, re3);
    EXPECT_EQ(re1, re4);
}

TEST_F(RegexTests, RegexpUnion)
{
    Regexp re1 = parser_.parse("[aeiou]");
    Regexp re2 = parser_.parse("[A-Z_a-z_0-9]");
    Regexp re3 = parser_.parse("[aeiou]|[A-Z_a-z_0-9]");
    Regexp re4 = unify(re1, re2);
    EXPECT_EQ(re3, re4);
}
