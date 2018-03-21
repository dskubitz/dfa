#include <gtest/gtest.h>
#include <Derivative.h>
#include <Parser.h>
#include <PrettyPrinter.h>

Parser parser;

TEST(Nullable, Test)
{
    auto re = parser.parse("a*");
    EXPECT_TRUE(Nullable{}.evaluate(re.get()));
    re = parser.parse("(a*|b)");
    EXPECT_TRUE(Nullable{}.evaluate(re.get()));
    re = parser.parse("a|b");
    EXPECT_FALSE(Nullable{}.evaluate(re.get()));
    re = parser.parse("(a*b)");
    EXPECT_FALSE(Nullable{}.evaluate(re.get()));
    re = parser.parse("ab");
    EXPECT_FALSE(Nullable{}.evaluate(re.get()));
    re = parser.parse("");
    EXPECT_TRUE(Nullable{}.evaluate(re.get()));
    re = std::make_unique<EmptyNode>();
    EXPECT_FALSE(Nullable{}.evaluate(re.get()));
}


TEST(SimplifyRegex, Union)
{
    std::unique_ptr<ASTNode> re(
            make_union(new EmptyNode, new CharNode('a')));
    auto p = dynamic_cast<CharNode*>(re.get());
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->value(), 'a');
    re.reset(make_union(new CharNode('a'), new CharNode('a')));
    p = dynamic_cast<CharNode*>(re.get());
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->value(), 'a');
}

TEST(SimplifyRegex, Cat)
{
    std::unique_ptr<ASTNode> re(
            make_cat(new EmptyNode, new CharNode('a')));
    auto p = dynamic_cast<EmptyNode*>(re.get());
    ASSERT_NE(p, nullptr);
    re.reset(make_cat(new EpsilonNode, new CharNode('a')));
    auto q = dynamic_cast<CharNode*>(re.get());
    ASSERT_NE(q, nullptr);
    EXPECT_EQ(q->value(), 'a');
}

TEST(SimplifyRegex, Star)
{
    std::unique_ptr<ASTNode> re(
            make_star(new StarNode(new StarNode(new CharNode('a')))));
    auto p = dynamic_cast<StarNode*>(re.get());
    ASSERT_NE(p, nullptr);
    auto q = dynamic_cast<const CharNode*>(p->expr());
    ASSERT_NE(q, nullptr);
}

TEST(SimplifyRegex, Intersection)
{
    std::unique_ptr<ASTNode> re(
            make_intersection(new EmptyNode, new CharNode('a')));
    auto p = dynamic_cast<EmptyNode*>(re.get());
    ASSERT_NE(p, nullptr);
    re.reset(make_intersection(new CharNode('a'), new CharNode('a')));
    auto p2 = dynamic_cast<CharNode*>(re.get());
    ASSERT_NE(p2, nullptr);
    EXPECT_EQ(p2->value(), 'a');
}

TEST(SimplifyRegex, Complement)
{
    std::unique_ptr<ASTNode> re(
            make_complement(new ComplementNode(new CharNode('a'))));
    auto p = dynamic_cast<CharNode*>(re.get());
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->value(), 'a');
}

TEST(RegexCompare, Test)
{
    auto re1 = parser.parse("");
    auto re2 = parser.parse("a");
    auto re3 = parser.parse("");
    EXPECT_NE(*re1, *re2);
    EXPECT_EQ(*re1, *re3);
    auto re4 = parser.parse("(a|b)*abb");
    auto re5 = parser.parse("(ab*|cd*)");
    EXPECT_NE(*re4, *re5);
    auto re6 = parser.parse("(a|b)*abb");
    EXPECT_EQ(*re4, *re6);
}

class DerivativeTests : public ::testing::Test {
protected:
    Derivative derivative;
};

TEST_F(DerivativeTests, BasicFunctionality)
{
    auto re1 = parser.parse("ab*");
    auto re2 = derivative.derive(re1.get());
    auto re3 = parser.parse("b*");
    EXPECT_EQ(*re1, *re2);
    re2 = derivative.derive(re1.get(), 'a');
    EXPECT_EQ(*re2, *re3);
}

TEST_F(DerivativeTests, MatchSimple)
{
    Nullable nullable;
    auto re = parser.parse("abc+");
    auto r = derivative.derive(re.get());
    EXPECT_FALSE(nullable.evaluate(r.get()));
    r = derivative.derive(r.get(), 'a');
    EXPECT_FALSE(nullable.evaluate(r.get()));
    r = derivative.derive(r.get(), 'b');
    EXPECT_FALSE(nullable.evaluate(r.get()));
    r = derivative.derive(r.get(), 'c');
    EXPECT_TRUE(nullable.evaluate(r.get()));
    r = derivative.derive(r.get(), 'c');
    EXPECT_TRUE(nullable.evaluate(r.get()));
    r = derivative.derive(r.get(), 'a');
    EXPECT_FALSE(nullable.evaluate(r.get()));
}
