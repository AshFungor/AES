// util
#include <cstdlib>
#include <definitions.hpp>

// standard
#include <iostream>
#include <numeric>
#include <ratio>
#include <stdexcept>
#include <utility>

// local
#include "numeric.hpp"

byte util::xtimes(const byte& src) {
    ushort product = src;
    product <<= 1;
    if (product & 0x0100) {
        return static_cast<byte>(product ^ m_x);
    }
    return static_cast<byte>(product);
}

byte util::multiply(byte first, byte second) {
    byte rotating = first;
    first *= (second & 1);
    while (second >>= 1) {
        rotating = util::xtimes(rotating);
        if (second & 1) {
            first = util::add(first, rotating);
        }
    }
    return first;
}

byte util::add(const byte& first, const byte& second) {
    return first ^ second;
}

byte util::inverse(byte src) {
    if (!src) {
        return 0;
    }

    byte iter = src;
    while (auto product = multiply(src, iter)) {
        if (product == 1) {
            return src;
        }
        src = product;
    }

    throw std::runtime_error("product of multiplication was 0!");
}

byte util::rotate(const byte& src) {
    byte border = src & 1;
    if (border) {
        return (src >> 1) | 0x80;
    }
    return src >> 1;
}

byte util::sum_bits(const byte& src) {
    byte result = 0;
    for (int bit = 0; bit < 8; ++bit) {
        result ^= (src >> bit) & 1;
    }
    return result;
}
