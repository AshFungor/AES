#pragma once

// standard
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <cstdint>
#include <ostream>
#include <string>
#include <array>

// Number of bytes in one column of state, defined by AES standard
inline constexpr std::size_t Nb = 4;

using byte = std::uint8_t;
using ushort = std::uint16_t;

// irreducible polynomial of degree 8, defined by AES standard
inline constexpr ushort m_x = 0x011B;

// constant for affine transformation, defined by AES standard
inline constexpr byte c = 0x63;

// 4-byte block, used widely
class word {
public:
    // default list-initializer constructor
    word(std::initializer_list<byte> bytes) {
        for (const auto& wbyte : bytes) {
            storage_ <<= 8;
            storage_ |= wbyte;
        }
    }

    word() = default;
    word(const word& other) = default;
    word(word&& other) = default;

    // range-safe access to bytes
    inline byte at(int index) {
        if (0 < index && index < Nb) {
            return (*this)[index];
        }
        throw std::runtime_error("index out of range: " + std::to_string(index));
    }

    // fast access to bytes
    inline byte operator[](int index) {
        return (storage_ >> (index * 8)) & 0xFF;
    }

    // XOR (means addition in terms of words)
    inline word& add(const word& other) {
        storage_ ^= other.storage_;
        return *this;
    }

    // rotation of word bytes
    inline word& rotate_left(int n = 1) {
        if (!n) return *this;
        constexpr auto shift = ((Nb - 1) * 8);
        byte tail = (storage_ & (0xFF << shift)) >> shift;
        storage_ = (storage_ << 8) | tail;
        return rotate_left(n - 1);
    }

    inline word& rotate_right(int n = 1) {
        if (!n) return *this;
        constexpr auto shift = ((Nb - 1) * 8);
        byte tail = storage_ & 0xFF;
        storage_ = (storage_ >> 8) | (tail << shift);
        return rotate_right(n - 1);
    }

    // output handler
    friend std::ostream& operator<<(std::ostream&, const word&);

private:
    std::uint32_t storage_{0};
};

inline std::ostream& operator<<(std::ostream& os, const word& buffer) {
    return os << std::hex << buffer.storage_ << std::dec;
}