#pragma once

// util
#include <concepts>
#include <cstddef>
#include <definitions.hpp>
#include <numeric.hpp>

// standard
#include <algorithm>
#include <cstdint>
#include <array>
#include <vector>
#include <iostream>

using namespace util;

namespace AES {

    using State = std::array<word, 4>;
    using Block = std::array<word, 4>;

    template<std::size_t BitMode>
    struct Key {
        enum class Mode : std::uint8_t {
            AES_128 = 10, AES_192 = 12, AES_256 = 14
        };

        Mode mode;
        std::array<byte, BitMode / 8> raw;
    };

    template<std::size_t BitMode>
    Block cipher(const Block& in, Key<BitMode> key);

    template<std::size_t BitMode>
    Block extract_round_key(std::vector<word>& routine, std::size_t start, std::size_t end);

    void add_round_key(State& state, Block block);
    void sub_bytes(State& state);
    void mix_columns(State& state);
    void shift_rows(State& state);

    byte sub_byte(byte src);

    // Keygen routine
    void sub_word(word& word);
    word match_rcon(int round);

    template<std::size_t BitMode>
    std::vector<word> key_extension(Key<BitMode> key);

} // namespace AES

template<std::size_t BitMode>
AES::Block AES::cipher(const Block &in, Key<BitMode> key) {
    State state {};

    auto routine = key_extension(key);

    std::copy(in.begin(), in.end(), state.begin());
    add_round_key(state, extract_round_key<BitMode>(routine, 0, Nb));

    for (std::size_t round = 1; round <= ((int) key.mode) - 1; ++round) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, extract_round_key<BitMode>(routine, round * Nb, (round + 1) * Nb));
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, extract_round_key<BitMode>(routine, ((int) key.mode) * Nb, ((int) key.mode + 1) * Nb));

    Block out {};
    std::copy(state.begin(), state.end(), out.begin());
    return out;
}

template<std::size_t BitMode>
AES::Block AES::extract_round_key(std::vector<word>& routine, std::size_t start, std::size_t end) {
    Block result;
    std::copy(std::next(routine.begin(), start), std::next(routine.begin(), end), result.begin());
    return std::move(result);
}

template<std::size_t BitMode>
std::vector<word> AES::key_extension(Key<BitMode> key) {
    std::vector<word> result;
    constexpr int Nk = BitMode / 8 / 4;

    int i = 0;
    word temp;
    while (i < Nk) {
        std::copy(std::next(key.raw.begin(), i * 4), std::next(key.raw.begin(), i * 4 + 4), temp.begin());
        result.push_back(std::move(temp));
        ++i;
    }

    while (i < Nb * ((int) key.mode + 1)) {
        temp = result.back();
        if (i % Nk == 0) {
            util::rotate_buffer_left(temp);
            sub_word(temp);
            auto rcon = match_rcon(i / Nk);
            util::add(temp, rcon);
        } else if (Nk > 6 && i % Nk == 4) {
            sub_word(temp);
        }
        util::add(temp, *std::next(result.end(), -Nk));
        result.push_back(temp);
        ++i;
    }
    return result;
}
