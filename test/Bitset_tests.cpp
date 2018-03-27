#include <gtest/gtest.h>
#include <Bitset.h>

TEST(BitsetTests, SizeofBitsetEqualsSixteen)
{
    Bitset set;
    EXPECT_EQ(sizeof(set), 16);
}

TEST(BitsetTests, TestBit)
{
    Bitset set(256);
    EXPECT_FALSE(set.test(7));
    EXPECT_TRUE(set.test(8));
    EXPECT_FALSE(set.test(9));
}

TEST(BitsetTests, SetBits)
{
    Bitset set;
    set.set();
    for (size_t i = 0; i < set.size(); ++i) {
        EXPECT_TRUE(set.test(i));
    }
    set.set(8, false);
    EXPECT_TRUE(set.test(7));
    EXPECT_FALSE(set.test(8));
    EXPECT_TRUE(set.test(9));
    set.set(8, true);
    set.set(9, false);
    EXPECT_TRUE(set.test(8));
    EXPECT_FALSE(set.test(9));
}
