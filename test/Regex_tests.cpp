#include <gtest/gtest.h>
#include <ASTNode.h>
#include <Parser.h>
#include <PrettyPrinter.h>

class RegexTests : public ::testing::Test {
protected:
    Parser parser;

};
TEST_F(RegexTests, Cat)
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

TEST_F(RegexTests, Union)
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

TEST_F(RegexTests, Star)
{
    std::unique_ptr<ASTNode> re(
            make_star(new StarNode(new StarNode(new CharNode('a')))));
    auto p = dynamic_cast<StarNode*>(re.get());
    ASSERT_NE(p, nullptr);
    auto q = dynamic_cast<const CharNode*>(p->expr());
    ASSERT_NE(q, nullptr);
}

TEST_F(RegexTests, Intersection)
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

TEST_F(RegexTests, Complement)
{
    std::unique_ptr<ASTNode> re(
            make_complement(new ComplementNode(new CharNode('a'))));
    auto p = dynamic_cast<CharNode*>(re.get());
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->value(), 'a');
}

TEST_F(RegexTests, Test)
{
    auto re1 = parser.parse("");
    auto re2 = parser.parse("a");
    auto re3 = parser.parse("");
    EXPECT_NE(re1->compare(re2.get()), 0);
    EXPECT_FALSE(*re1 == *re2);

    EXPECT_EQ(re1->compare(re3.get()), 0);
    EXPECT_EQ(*re1, *re3);

    auto re4 = parser.parse("(a|b)*abb");
    auto re5 = parser.parse("(ab*|cd*)");
    EXPECT_NE(re4->compare(re5.get()), 0);
    EXPECT_NE(*re4, *re5);

    auto re6 = parser.parse("(a|b)*abb");
    EXPECT_EQ(re4->compare(re6.get()), 0);
    EXPECT_TRUE(*re4 == *re6);
}

