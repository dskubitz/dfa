#include <gtest/gtest.h>
#include <Derivative.h>
#include <Parser.h>
#include <Nullable.h>

class DerivativeTests : public ::testing::Test {
protected:
    Derivative derivative;
    Parser parser;
};

TEST_F(DerivativeTests, BasicFunctionality)
{
    auto re1 = parser.parse("ab*");
    auto re2 = derivative.derive(re1);
    auto re3 = parser.parse("b*");
    EXPECT_EQ(re1, re2);
    re2 = derivative.derive(re1, 'a');
    EXPECT_EQ(re2, re3);
    auto re4 = parser.parse("(a|b)*b");
    auto r = derivative.derive(re4);
    std::cout << r.get()->to_string() << '\n';
    r = derivative.derive(re4, 'a');
    std::cout << r.get()->to_string() << '\n';

}

TEST_F(DerivativeTests, MatchSimple)
{
    Nullable nullable;
    auto re = parser.parse("abc+");
    auto r = derivative.derive(re);
    EXPECT_FALSE(nullable.evaluate(r.get()));
    r = derivative.derive(r, 'a');
    EXPECT_FALSE(nullable.evaluate(r.get()));
    r = derivative.derive(r, 'b');
    EXPECT_FALSE(nullable.evaluate(r.get()));
    r = derivative.derive(r, 'c');
    EXPECT_TRUE(nullable.evaluate(r.get()));
    r = derivative.derive(r, 'c');
    EXPECT_TRUE(nullable.evaluate(r.get()));
    r = derivative.derive(r, 'a');
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
}
