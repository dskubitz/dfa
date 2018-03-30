#include <gtest/gtest.h>
#include <DerivativeEvaluator.h>
#include <DerivativeClassEvaluator.h>
#include <Parser.h>
class DerivativeTests : public ::testing::Test {
protected:
    DerivativeEvaluator derivative;
    Parser parser;
};

TEST_F(DerivativeTests, BasicFunctionality)
{
    auto re1 = parser.parse("ab*");
    auto re2 = re1;
    auto re3 = parser.parse("b*");
    EXPECT_EQ(re1, re2);
    re2 = derivative.derive(re1, 'a');
    EXPECT_EQ(re2, re3);
}

TEST_F(DerivativeTests, MatchSimple)
{
    NullableEvaluator nullable;
    auto re = parser.parse("abc+");
    auto r = re;
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
    EXPECT_TRUE(NullableEvaluator{}.evaluate(re.get()));
    re = parser.parse("(a*|b)");
    EXPECT_TRUE(NullableEvaluator{}.evaluate(re.get()));
    re = parser.parse("a|b");
    EXPECT_FALSE(NullableEvaluator{}.evaluate(re.get()));
    re = parser.parse("(a*b)");
    EXPECT_FALSE(NullableEvaluator{}.evaluate(re.get()));
    re = parser.parse("ab");
    EXPECT_FALSE(NullableEvaluator{}.evaluate(re.get()));
    re = parser.parse("");
    EXPECT_TRUE(NullableEvaluator{}.evaluate(re.get()));
}

TEST_F(DerivativeTests, RVectorTests)
{
    DerivativeClassEvaluator derivativeClass;
    auto re = Parser{}.parse("[A-Za-z_][A-Za-z_0-9]*");
    auto re2 = Parser{}.parse("[0-9]+(\\.[0-9]+)?");
    auto set1 = derivativeClass.evaluate(re);
    auto set2 = derivativeClass.evaluate(re);
    std::vector<Regexp> regular_vector{re, re2};
    auto set = make_derivative_class(regular_vector);
}
