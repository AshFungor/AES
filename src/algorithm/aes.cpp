// util
#include <definitions.hpp>
#include <numeric.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

#include "aes.hpp"

void AES::sub_bytes(State& state) {
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < Nb; ++column) {
            state[row][column] = sub_byte(state[row][column]);
        }
    }
}

void AES::shift_rows(State& state) {
    for (int row = 0; row < 4; ++row) {
        auto shift = row;
        while (shift--) {
            byte curr = state[0][row];
            for (int i = Nb - 1; i >= 0; --i) {
                std::swap(curr, state[i][row]);
            }
        }
    }
}

void AES::mix_columns(State &state) {
    word multiplicand {0x02, 0x03, 0x01, 0x01};

    for (int column = 0; column < Nb; ++column) {
        word newColumn;
        for (int row = 0; row < 4; ++row) {
            byte result = 0;
            for (int cbyte = 0; cbyte < Nb; ++cbyte) {
                result = util::add(
                    result,
                    util::multiply(
                        state[column][cbyte],
                        multiplicand[cbyte]));
            }
            multiplicand.rotate_right();
            newColumn[row] = result;
        }
        state[column] = newColumn;
    }
}

void AES::add_round_key(State& state, Block block) {
    for (int column = 0; column < Nb; ++column) {
        for (int row = 0; row < 4; ++row) {
            state[row][column] ^= block[row][column];
        }
    }
}

byte AES::sub_byte(byte src) {
    byte multiplicand = 0xF8;
    src = util::inverse(src);
    byte result = 0;
    for (int bit = 7; bit >= 0; --bit) {
        result |= util::sum_bits(src & multiplicand) << bit;
        multiplicand = util::rotate(multiplicand);
    }
    return util::add(result, c);
}

void AES::sub_word(word& word) {
    for (int i = 0; i < 4; ++i) {
        word[i] = sub_byte(word[i]);
    }
}

word AES::match_rcon(int round) {
    word result {};
    switch (round) {
        case 1: result[0] = 0x01; return result;
        case 2: result[0] = 0x02; return result;
        case 3: result[0] = 0x04; return result;
        case 4: result[0] = 0x08; return result;
        case 5: result[0] = 0x10; return result;
        case 6: result[0] = 0x20; return result;
        case 7: result[0] = 0x40; return result;
        case 8: result[0] = 0x80; return result;
        case 9: result[0] = 0x1B; return result;
        case 10: result[0] = 0x36; return result;
        default:
            throw std::runtime_error("round value must be in range [1, 10], given: " + std::to_string(round));
    }
}
