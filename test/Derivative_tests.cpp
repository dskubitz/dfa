#include <gtest/gtest.h>
#include <Derivative.h>
#include <Parser.h>
#include <PrettyPrinter.h>

class DerivativeTests : public ::testing::Test {
protected:
    Derivative derivative;
    Parser parser;
};

TEST_F(DerivativeTests, BasicFunctionality)
{
    auto re1 = parser.parse("ab*");
    auto re2 = derivative.derive(re1.get());
    auto re3 = parser.parse("b*");
    EXPECT_EQ(re1->compare(re2.get()), 0);
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

TEST_F(DerivativeTests, Nullable)
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
