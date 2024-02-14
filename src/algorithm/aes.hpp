#pragma once

// util
#include <concepts>
#include <cstddef>
#include <definitions.hpp>
#include <iterator>
#include <numeric.hpp>

// standard
#include <algorithm>
#include <cstdint>
#include <array>
#include <vector>
#include <iostream>

using namespace util;

namespace AES {

    using State = std::array<word, Nb>;
    using Block = std::array<word, Nb>;

    enum class Mode : int {
        AES_128 = 128, AES_192 = 192, AES_256 = 256
    };

    template<Mode BitMode>
    struct Key {
        const std::size_t Nr = (int) BitMode;
        std::array<byte, (int) BitMode / 8> raw;
    };

    template<Mode BitMode>
    Block cipher(const Block& in, Key<BitMode> key);

    template<Mode BitMode>
    Block extract_round_key(std::vector<word>& routine, std::size_t start);

    void add_round_key(State& state, Block block);
    void sub_bytes(State& state);
    void mix_columns(State& state);
    void shift_rows(State& state);

    byte sub_byte(byte src);

    // Keygen routine
    void sub_word(word& word);
    word match_rcon(int round);

    template<Mode BitMode>
    std::vector<word> key_extension(Key<BitMode> key);

} // namespace AES

template<AES::Mode BitMode>
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

template<AES::Mode BitMode>
AES::Block AES::extract_round_key(std::vector<word>& routine, std::size_t start) {
    Block result;
    const auto begin = std::next(routine.begin(), start);
    std::copy(begin, begin + Nb, result.begin());
    return std::move(result);
}

template<AES::Mode BitMode>
std::vector<word> AES::key_extension(Key<BitMode> key) {
    std::vector<word> result;
    constexpr int Nk = (int) BitMode / 8 / 4;

    int i = 0;
    word temp;
    while (i < Nk) {
        for (int wbyte = 0; wbyte < 4; ++wbyte) {
            temp[wbyte] = key.raw[i * 4 + wbyte];
        }
        result.push_back(std::move(temp));
        ++i;
    }

    while (i < Nb * ((int) key.mode + 1)) {
        temp = result.back();
        if (i % Nk == 0) {
            temp.rotate_left();
            sub_word(temp);
            auto rcon = match_rcon(i / Nk);
            temp.add(rcon);
        } else if (Nk > 6 && i % Nk == 4) {
            sub_word(temp);
        }
        temp.add(*std::next(result.end(), -Nk));
        result.push_back(temp);
        ++i;
    }
    return result;
}
