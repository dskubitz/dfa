#include <gtest/gtest.h>
#include <lexer/derivative.h>
#include <lexer/parser.h>

class DerivativeTests : public ::testing::Test {
protected:
    derivative derivative_;
    parser parser_;
};

TEST_F(DerivativeTests, BasicFunctionality)
{
    auto re1 = parser_.parse("ab*");
    auto re2 = re1;
    auto re3 = parser_.parse("b*");
    EXPECT_EQ(re1, re2);
    re2 = derivative_.derive(re1, 'a');
    EXPECT_EQ(re2, re3);
}

TEST_F(DerivativeTests, MatchSimple)
{
    Nullable nullable;
    auto re = parser_.parse("abc+");
    auto r = re;
    EXPECT_FALSE(nullable.evaluate(r.get()));
    r = derivative_.derive(r, 'a');
    EXPECT_FALSE(nullable.evaluate(r.get()));
    r = derivative_.derive(r, 'b');
    EXPECT_FALSE(nullable.evaluate(r.get()));
    r = derivative_.derive(r, 'c');
    EXPECT_TRUE(nullable.evaluate(r.get()));
    r = derivative_.derive(r, 'c');
    EXPECT_TRUE(nullable.evaluate(r.get()));
    r = derivative_.derive(r, 'a');
    EXPECT_FALSE(nullable.evaluate(r.get()));
}

TEST_F(DerivativeTests, Nullable)
{
    auto re = parser_.parse("a*");
    EXPECT_TRUE(Nullable{}.evaluate(re.get()));
    re = parser_.parse("(a*|b)");
    EXPECT_TRUE(Nullable{}.evaluate(re.get()));
    re = parser_.parse("a|b");
    EXPECT_FALSE(Nullable{}.evaluate(re.get()));
    re = parser_.parse("(a*b)");
    EXPECT_FALSE(Nullable{}.evaluate(re.get()));
    re = parser_.parse("ab");
    EXPECT_FALSE(Nullable{}.evaluate(re.get()));
    re = parser_.parse("");
    EXPECT_TRUE(Nullable{}.evaluate(re.get()));
}

TEST_F(DerivativeTests, RVectorTests)
{
    DerivativeClass derivativeClass;
    auto re = parser{}.parse("[A-Za-z_][A-Za-z_0-9]*");
    auto re2 = parser{}.parse("[0-9]+(\\.[0-9]+)?");
    auto set1 = derivativeClass.evaluate(re);
    auto set2 = derivativeClass.evaluate(re);
    std::vector<regexp> regular_vector{re, re2};
    auto set = make_derivative_class(regular_vector);
}

TEST_F(DerivativeTests, Complement)
{
    auto re = parser{}.parse("~abc");
    auto set = make_derivative_class({re});
    for (auto& i : set) {
        auto deriv = derivative{}.derive(re, first_occurring(i));
        std::cout << deriv << '\n';
    }
}
