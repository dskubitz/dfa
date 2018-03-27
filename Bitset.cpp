#include <Bitset.h>
#include <stdexcept>

bool Bitset::test(size_t pos) const
{
    if (pos >= 128)
        throw std::out_of_range("Bitset::test");
    return llw[pos / 64] & (1 << (pos % 64));
}

Bitset& Bitset::set()
{
    llw[0] = static_cast<size_t>(-1);
    llw[1] = static_cast<size_t>(-1);
    return *this;
}

Bitset& Bitset::set(size_t pos, bool value)
{
    if (pos >= 128)
        throw std::out_of_range("Bitset::set");
    auto mask = 1ULL << (pos % 64ULL);
    auto& ref = llw[pos / 64ULL];
    ref ^= (-value ^ ref) & mask;
    return *this;
}
