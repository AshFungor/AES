// Gtest
#include <gtest/gtest.h>
#include <stdexcept>

// Util
#define private public
#include <definitions.hpp>
#include <numeric.hpp>
#undef private

// standard
#include <iostream>
#include <initializer_list>

#define GTEST_COUT(chain) std::cerr << "[INFO      ] " << chain << '\n'

TEST(WordApiTests, Constructors) {
    // Initializer list
    word firstBuffer {1, 2, 3, 4};
    GTEST_COUT("byte alignment for input array [1, 2, 3, 4]: " << firstBuffer);

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

TEST(WordApiTests, IndexAccess) {
    std::initializer_list<byte> source {1, 2, 3, 4};
    word buffer {source};

    // basic iteration over buffer
    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(*(source.begin() + i), buffer[i]);
        EXPECT_EQ(*(source.begin() + i), buffer.at(i));
    }

    // expect range-check for at() method
    ASSERT_THROW(buffer.at(4), std::runtime_error);

    // rotation
    const word rotated_left = {2, 3, 4, 1};
    ASSERT_EQ(buffer.rotate_left(), rotated_left);
    // invert rotation
    buffer.rotate_right();
    const word rotated_right = {4, 1, 2, 3};
    ASSERT_EQ(buffer.rotate_right(), rotated_right);
    buffer.rotate_left();

    const word rotated_left_by_3 = {4, 1, 2, 3};
    ASSERT_EQ(buffer.rotate_left(3), rotated_left_by_3);
    buffer.rotate_right(3);
    const word rotated_right_by_3 = {2, 3, 4, 1};
    ASSERT_EQ(buffer.rotate_right(3), rotated_right_by_3);
}

TEST(ByteOpsTests, ByteOps) {
    // multiplication
    byte first = 0x57, second = 0x83;
    ASSERT_EQ(util::multiply(first, second), 0xC1);
    second = 0x13;
    ASSERT_EQ(util::multiply(first, second), 0xFE);

    // rotation
    first = 0xC0;
    ASSERT_EQ(util::rotate(first), 0x60);

    // inversion
    ASSERT_EQ(util::inverse(0), 0);
    ASSERT_EQ(util::inverse(0x53), 0xCA);
}
