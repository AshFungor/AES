// Gtest
#include <gtest/gtest.h>

// algorithm
#include <aes.hpp>

// util
#include <definitions.hpp>
#include <text.hpp>

// standard
#include <memory>
#include <vector>

namespace {
    bool matchPrefix(std::vector<word> first, std::vector<word> second) {
        auto common = std::min(first.size(), second.size());
        for (int i = 0; i < common; ++i) {
            if (first[i] != second[i]) {
                return false;
            }
        }
        return true;
    }
}

class AESTest : public testing::Test {
protected:
    std::shared_ptr<Block> in;
    std::shared_ptr<State> state;
    std::shared_ptr<Key<Mode::AES_128>> key_128;
    std::shared_ptr<Key<Mode::AES_192>> key_192;
    std::shared_ptr<Key<Mode::AES_256>> key_256;

    void SetUp() override {
        const auto input = "This is the best";
        in = std::make_shared<Block>(text::parseText(input));
        state = std::make_shared<State>();

        // 16 letters
        const auto inputKey_128 = "This is the best";
        key_128 = std::make_shared<Key<Mode::AES_128>>(text::parseKey<Mode::AES_128>(inputKey_128));
        // 24 letters
        const auto inputKey_192 = "This is the best, really";
        key_192 = std::make_shared<Key<Mode::AES_192>>(text::parseKey<Mode::AES_192>(inputKey_192));
        // 32 letters
        const auto inputKey_256 = "This is the best, for real? no..";
        key_256 = std::make_shared<Key<Mode::AES_256>>(text::parseKey<Mode::AES_256>(inputKey_256));
    }
};

TEST_F(AESTest, SubBytes) {
    state = std::make_shared<State>(State{
        word{0x00, 0x01, 0x02, 0x03},
        word{0x04, 0x05, 0x06, 0x07},
        word{0x08, 0x09, 0x0A, 0x0B},
        word{0x0C, 0x0D, 0x0E, 0x0F}
    });

    AES::sub_bytes(*state);

    State sboxExpected {
        word{0x63, 0x7C, 0x77, 0x7B},
        word{0xF2, 0x6B, 0x6F, 0xC5},
        word{0x30, 0x01, 0x67, 0x2B},
        word{0xFE, 0xD7, 0xAB, 0x76}
    };

    ASSERT_EQ(sboxExpected, *state);
}

TEST_F(AESTest, MixColumns) {
    state = std::make_shared<State>(State{
        word{0x63, 0x2F, 0xAF, 0xA2},
        word{0xEB, 0x93, 0xC7, 0x20},
        word{0x9F, 0x92, 0xAB, 0xCB},
        word{0xA0, 0xC0, 0x30, 0x2B}
    });

    AES::mix_columns(*state);

    State mixedColumnsExpected {
        word{0xBA, 0x75, 0xF4, 0x7A},
        word{0x84, 0xA4, 0x8D, 0x32},
        word{0xE8, 0x8D, 0x06, 0x0E},
        word{0x1B, 0x40, 0x7D, 0x5D}
    };

    ASSERT_EQ(mixedColumnsExpected, *state);
}

TEST_F(AESTest, shiftRows) {
    state = std::make_shared<State>(State{
        word{0x63, 0xC0, 0xAB, 0x20},
        word{0xEB, 0x2F, 0x30, 0xCB},
        word{0x9F, 0x93, 0xAF, 0x2B},
        word{0xA0, 0x92, 0xC7, 0xA2}
    });

    AES::shift_rows(*state);

    Block shiftedRowsExpected = {
        word{0x63, 0x2F, 0xAF, 0xA2},
        word{0xEB, 0x93, 0xC7, 0x20},
        word{0x9F, 0x92, 0xAB, 0xCB},
        word{0xA0, 0xC0, 0x30, 0x2B}
    };

    ASSERT_EQ(shiftedRowsExpected, *state);
}

TEST_F(AESTest, KeyExpansion128) {
    key_128 = std::make_shared<Key<Mode::AES_128>>(Key<Mode::AES_128>{
        0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
    });

    auto result = AES::key_extension<Mode::AES_128>(*key_128);

    std::vector<word> expected {
        word(0x2B7E1516),
        word(0x28AEd2A6),
        word(0xABf71588),
        word(0x09CF4F3C),
        word(0xA0FAFE17),
        word(0x88542CB1),
        word(0x23A33939),
        word(0x2A6C7605)
    };

    ASSERT_TRUE(matchPrefix(result, expected)) << "prefix of resulting round keys did not match expected";
}

TEST_F(AESTest, Cipher){
    in = std::make_shared<Block>(Block{
        word(0x3243F6A8),
        word(0x885A308D),
        word(0x313198A2),
        word(0xE0370734)
    });

    key_128 = std::make_shared<Key<Mode::AES_128>>(Key<Mode::AES_128>{
        0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
    });

    auto result = AES::cipher(*in, *key_128);

    Block outExpected {
        word(0x3925841D),
        word(0x02DC09FB),
        word(0xDC118597),
        word(0x196A0B32)
    };

    ASSERT_EQ(outExpected, result);
}

TEST_F(AESTest, InvSubBytes) {
    state = std::make_shared<State>(State{
        word{0x00, 0x01, 0x02, 0x03},
        word{0x04, 0x05, 0x06, 0x07},
        word{0x08, 0x09, 0x0A, 0x0B},
        word{0x0C, 0x0D, 0x0E, 0x0F}
    });

    AES::inv_sub_bytes(*state);

    State invSboxExpected {
        word{0x52, 0x09, 0x6A, 0xD5},
        word{0x30, 0x36, 0xA5, 0x38},
        word{0xBF, 0x40, 0xA3, 0x9E},
        word{0x81, 0xF3, 0xD7, 0xFB}
    };

    ASSERT_EQ(invSboxExpected, *state);
}

TEST_F(AESTest, InvShiftRows) {
    state = std::make_shared<State>(State{
        word{0x00, 0x10, 0x20, 0x30},
        word{0x01, 0x11, 0x21, 0x31},
        word{0x02, 0x12, 0x22, 0x32},
        word{0x03, 0x13, 0x23, 0x33}
    });

    AES::inv_shift_rows(*state);

    Block shiftedRowsExpected = {
        word{0x00, 0x13, 0x22, 0x31},
        word{0x01, 0x10, 0x23, 0x32},
        word{0x02, 0x11, 0x20, 0x33},
        word{0x03, 0x12, 0x21, 0x30}
    };
}

TEST_F(AESTest, Decipher){
    in = std::make_shared<Block>(Block{
        word(0x3243F6A8),
        word(0x885A308D),
        word(0x313198A2),
        word(0xE0370734)
    });

    key_128 = std::make_shared<Key<Mode::AES_128>>(Key<Mode::AES_128>{
        0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
    });

    auto result = AES::cipher(*in, *key_128);
    auto inverted = AES::decipher(result, *key_128);

    ASSERT_EQ(*in, inverted);
}
