#pragma once

// standard
#include <cstdint>
#include <array>

// Number of bytes in one column of state, defined by AES standard
inline constexpr std::size_t Nb = 4;

using byte = std::uint8_t;
using ext_byte = std::uint16_t;
using word = std::array<byte, Nb>;

// irreducible polynomial of degree 8, defined by AES standard
inline constexpr ext_byte m_x = 0x011B;

// constant for affine transformation, defined by AES standard
inline constexpr byte c = 0x63;