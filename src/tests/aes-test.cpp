// Gtest
#include <gtest/gtest.h>

// algorithm
#include <aes.hpp>

// util
#include <text.hpp>

// standard
#include <memory>


class AESTest : testing::Test {
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
        const auto input_key_128 = "This is the sad";
        // 24 letters 
        const auto input_key_192 = "This is the best, real!";
        // 32 letters 
        const auto input_key_256 = "This is the best, at last it..."; 
    } 
};