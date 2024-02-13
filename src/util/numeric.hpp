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
} // namespace util
