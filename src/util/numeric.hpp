#pragma once

#include <definitions.hpp>

// standard
#include <cstdint>
#include <ostream>

namespace util {
    // bits ops
    byte xtimes(const byte& src);
    byte nxtimes(byte src, int n);
    byte multiply(byte first, byte second);
    byte add(const byte& first, const byte& second);
    byte rotate(const byte& src);
    byte sum_bits(const byte& src);

    // inverse
    byte inverse(byte src);

    // bytes ops
    void add(word& src, const word& addition);
    void multiply_no_inv(word& src, const word& multiplicand);

    // buffer ops
    void rotate_buffer_right(word& buf);
    void rotate_buffer_left(word& buf);

    std::uint32_t join_word(const word& buf);
} // namespace util
