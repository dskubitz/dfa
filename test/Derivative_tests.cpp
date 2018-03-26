#include <gtest/gtest.h>
#include <Derivative.h>
#include <DerivativeClass.h>
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

TEST_F(DerivativeTests, DerivativeClassTest)
{
    auto re = Parser{}.parse("def|[abc][abc_01]*");
    Derivative D;
    DerivativeClass derivativeClass;

    auto cl = derivativeClass.evaluate(re);

    std::unordered_set<Regexp> set1;
    for (int i = 0; i < 128; ++i) {
        set1.insert(D.derive(re, i));
    }
    std::unordered_set<Regexp> set2;
    for (auto& set : cl) {
        int first_of = first(set);
        set2.insert(D.derive(re, first_of));
    }
    EXPECT_EQ(set1.size(), set2.size());
}

TEST_F(DerivativeTests, RVectorTests)
{
    DerivativeClass derivativeClass;
    auto re = Parser{}.parse("[A-Za-z_][A-Za-z_0-9]*");
    auto re2 = Parser{}.parse("[0-9]+(\\.[0-9]+)?");
    auto set1 = derivativeClass.evaluate(re);
    auto set2 = derivativeClass.evaluate(re);
    std::vector<Regexp> regular_vector{re, re2};
    auto set = make_derivative_class(regular_vector);
    /*
    print(regular_vector);
    for (auto& s : set) {
        auto c = static_cast<char>(first(s));
        auto rev = make_derivative(regular_vector, c);
        print(rev);
        std::cout << std::boolalpha << is_nullable(rev) << std::noboolalpha
                  << '\n';
    }
    */
}
