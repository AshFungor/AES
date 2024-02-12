// util
#include <cstdlib>
#include <definitions.hpp>

// standard
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <utility>

// local
#include "numeric.hpp"

byte util::xtimes(const byte& src) {
    ext_byte product = src;
    product <<= 1;
    if (product & 0x0100) {
        return static_cast<byte>(product ^ m_x);
    }
    return static_cast<byte>(product);
}

byte util::nxtimes(byte src, int n) {
    byte res = src;
    while (n--) {
        res = xtimes(res);
    }
    return res;
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

void util::add(word& src, const word& addition) {
    for (int i = 0; i < Nb; ++i) {
        src[i] ^= addition[i];
    }
}

void util::multiply_no_inv(word& src, const word& multiplicand) {
    word buffer {};
    std::iota(buffer.begin(), buffer.end(), 0);
    for (int i = 0; i < Nb; ++i) {
        rotate_buffer_right(buffer);
        byte c = 0;
        for (int j = 0; j < Nb; ++j) {
            c ^= multiply(buffer[j], multiplicand[j]);
        }
        src[i] = c;
    }
}

void util::rotate_buffer_right(word& buf) {
    byte tail = buf[0];
    for (int i = 1; i <= Nb; ++i) {
        std::swap(tail, buf[i % Nb]);
    }
}

void util::rotate_buffer_left(word& buf) {
    byte tail = buf[0];
    for (int i = Nb - 2; i >= -1; ++i) {
        std::swap(tail, buf[(i + Nb) % Nb]);
    }
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
        return (src >> 1) | 0b10000000;
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