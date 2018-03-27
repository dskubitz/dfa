#ifndef LEXER_BITSET_H
#define LEXER_BITSET_H

#include <cstdint>
#include <cstddef>

// std::bitset does not have find_first
// boost::dynamic_bitset is, well dynamic
// Don't need subscripting, just test and set, may as well do flip/any/all/none...
// and operator overloads. Fixing at 128 bits so implementation should be
// simple...
// Doc comments are taken from cppreference
class Bitset {
public:
    /**
     * Default constructor. Constructs a bitset with all bits set to zero.
     */
    constexpr Bitset() noexcept
            : llw()
    {
    }

    /**
     * This is for testing, may remove when other functions are implemented
     */
    constexpr Bitset(unsigned long long val) noexcept
            : Bitset()
    {
        llw[0] = val;
    }

    /**
     * Returns the number of bits that the bitset can hold, which
     * is sizeof(unsigned long long) * 2.
     *
     * @return number of bits that the bitset can hold.
     */
    constexpr size_t size() const noexcept { return 128; }

    /**
     * Returns the value of the bit at the position pos.
     * Throws std::out_of_range if pos does not correspond
     * to a valid position within the bitset.
     *
     * @param pos - position of the bit to return
     * @return true if the requested bit is set, false otherwise.
     */
    bool test(size_t pos) const;

    /**
     * Sets all bits to true
     * @return *this
     */
    Bitset& set();

    /**
     * Sets the bit at position pos to the value value.
     * Throws std::out_of_range if pos does not correspond
     * to a valid position within the bitset.
     *
     * @param pos 	the position of the bit to set
     *              (least significant to most significant)
     * @param value the value to set the bit to
     * @return *this
     */
    Bitset& set(size_t pos, bool value = true);

private:
    uint64_t llw[2];
};

#endif //LEXER_BITSET_H
