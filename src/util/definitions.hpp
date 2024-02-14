#pragma once

// standard
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <cstdint>
#include <ostream>
#include <string>
#include <array>

// Number of words in one row of state, defined by AES standard
inline constexpr std::size_t Nb = 4;

using byte = std::uint8_t;
using ushort = std::uint16_t;

// irreducible polynomial of degree 8, defined by AES standard
inline constexpr ushort m_x = 0x011B;

// constant for affine transformation, defined by AES standard
inline constexpr byte c = 0x63;

// Matrices defined in standard
class word;
using State = std::array<word, Nb>;
using Block = std::array<word, Nb>;

// Key modes, defined by standard
enum class Mode : int {
    AES_128 = 128, AES_192 = 192, AES_256 = 256
};

// Raw key struct
template<Mode BitMode>
struct Key {
    const std::size_t Nr = (int) BitMode;
    std::array<byte, (int) BitMode / 8> raw;
};

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
    word& operator=(const word& other) = default;

    // range-safe access to bytes
    inline byte& at(int index) {
        if (0 <= index && index < 4) {
            return (*this)[index];
        }
        throw std::runtime_error("index out of range: " + std::to_string(index));
    }

    // fast access to bytes
    inline byte& operator[](int index) {
        return reinterpret_cast<byte&>(*((byte*) &storage_ + (3 - index)));
    }

    // XOR (means addition in terms of words)
    inline word& add(const word& other) {
        storage_ ^= other.storage_;
        return *this;
    }

    // rotation of word bytes
    inline word& rotate_left(int n = 1) {
        if (!n) return *this;
        constexpr auto shift = ((4 - 1) * 8);
        std::uint32_t tail = (storage_ & (0xFF << shift)) >> shift;
        storage_ = (storage_ << 8) | tail;
        return rotate_left(n - 1);
    }

    inline word& rotate_right(int n = 1) {
        if (!n) return *this;
        constexpr auto shift = ((4 - 1) * 8);
        std::uint32_t tail = (storage_ & 0xFF) << shift;
        storage_ = (storage_ >> 8) | tail;
        return rotate_right(n - 1);
    }

    inline bool operator==(const word& other) const {
        return storage_ == other.storage_;
    }

    // output handler
    friend std::ostream& operator<<(std::ostream&, const word&);

private:
    std::uint32_t storage_{0};
};

inline std::ostream& operator<<(std::ostream& os, const word& buffer) {
    return os << std::hex << buffer.storage_ << std::dec;
}
