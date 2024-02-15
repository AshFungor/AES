// Gtest
#include "definitions.hpp"
#include <gtest/gtest.h>

// algorithm
#include <aes.hpp>

// util
#include <text.hpp>

// standard
#include <memory>


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
        word{0x63, 0xEB, 0x9F, 0xA0},
        word{0x2F, 0x93, 0x92, 0xC0},
        word{0xAF, 0xC7, 0xAB, 0x30},
        word{0xA2, 0x20, 0xCB, 0x2B}
    });

    AES::mix_columns(*state);

    State mixedColumnsExpected {
        word{0xBA, 0x84, 0xE8, 0x1B},
        word{0x75, 0xA4, 0x8D, 0x40},
        word{0xF4, 0x8D, 0x06, 0x7D},
        word{0x7A, 0x32, 0x0E, 0x5D}
    };

    ASSERT_EQ(mixedColumnsExpected, *state); 
}