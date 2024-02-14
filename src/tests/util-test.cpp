// Gtest
#include <gtest/gtest.h>

// Util
#define private public
#include <definitions.hpp>
#include <numeric.hpp>
#undef private

// standard
#include <iostream>

#define GTEST_COUT(chain) std::cerr << "[INFO] " << chain

TEST(WordApiTests, Constructors) {
    // Initializer list
    word firstBuffer {1, 2, 3, 4};
    GTEST_COUT(firstBuffer);

    // Copy constructor
    word secondBuffer {firstBuffer};
    ASSERT_EQ(firstBuffer.storage_, secondBuffer.storage_);

    // Move constructor
    word thirdBuffer {std::move(firstBuffer)};
    ASSERT_EQ(secondBuffer.storage_, thirdBuffer.storage_);

    // Assign
    firstBuffer = thirdBuffer;
    ASSERT_EQ(firstBuffer.storage_, thirdBuffer.storage_);
}
