// AES
#include <aes.hpp>

// util
#include <numeric.hpp>
#include <definitions.hpp>

// standard
#include <iostream>
#include <iomanip>

int main() {
    // byte a = 0x57;
    // std::cout << std::hex;
    // std::cout << "round 1: " << (int) (a = util::xtimes(a)) << '\n';
    // std::cout << "round 2: " << (int) (a = util::xtimes(a)) << '\n';
    // std::cout << "round 3: " << (int) (a = util::xtimes(a)) << '\n';
    // std::cout << "round 4: " << (int) (a = util::xtimes(a)) << '\n';

    // std::cout << "57 * 13 in hex is: " << std::hex << (int) util::multiply(0x57, 0x13) << '\n';

    // byte b = 0xFE;
    // std::cout << std::hex;
    // std::cout << (int) util::sum_bits(b) << '\n';

    // AES::State state {
    //     {{0x00, 0x01, 0x02, 0x03},
    //     {0x04, 0x05, 0x06, 0x07},
    //     {0x08, 0x09, 0x10, 0x11},
    //     {0x12, 0x13, 0x14, 0x15}}
    // };
    // AES::sub_bytes(state);
    // for (auto& row: state) {
    //     for (auto& el : row) {
    //         std::cout << (int) el << ' ';
    //     }
    //     std::cout << '\n';
    // }
    // std::cout << (int) util::inverse(0xca) << '\n';

    // auto key = AES::Key<128>{
    //     .mode = AES::Key<128>::Mode::AES_128,
    //     .raw = {
    //         0x00, 0x01, 0x02, 0x03,
    //         0x04, 0x05, 0x06, 0x07,
    //         0x08, 0x09, 0x10, 0x11,
    //         0x12, 0x13, 0x14, 0x15}
    // };

    // AES::Block in {
    //     {{0x00, 0x01, 0x02, 0x03},
    //     {0x04, 0x05, 0x06, 0x07},
    //     {0x08, 0x09, 0x10, 0x11},
    //     {0x12, 0x13, 0x14, 0x15}}
    // };

    // auto out = AES::cipher(in, key);

    // for (auto& row: out) {
    //     for (auto& el : row) {
    //         std::cout << (int) el << ' ';
    //     }
    //     std::cout << '\n';
    // }

    // 2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c
    // auto key = AES::Key<128>{
    //     .mode = AES::Key<128>::Mode::AES_128,
    //     .raw = {
    //         0x2b, 0x7e, 0x15, 0x16,
    //         0x28, 0xae, 0xd2, 0xa6,
    //         0xab, 0xf7, 0x15, 0x88,
    //         0x09, 0xcf, 0x4f, 0x3c}
    // };
    // std::cout << std::hex;
    // auto routine = AES::key_extension(key);
    // int i = 0;
    // for (auto& w : routine) {
    //     std::cout << "word" << i << ": ";
    //     for (auto& b : w) {
    //         std::cout << (int) b;
    //     }
    //     std::cout << '\n';
    //     ++i;
    // }

    // 8e 73 b0 f7 da 0e 64 52 c8 10 f3 2b 80 90 79 e5 62 f8 ea d2 52 2c 6b 7b
    // auto key = AES::Key<192>{
    //     .mode = AES::Key<192>::Mode::AES_192,
    //     .raw = {
    //         0x8e, 0x73, 0xb0, 0xf7,
    //         0xda, 0x0e, 0x64, 0x52,
    //         0xc8, 0x10, 0xf3, 0x2b,
    //         0x80, 0x90, 0x79, 0xe5,
    //         0x62, 0xf8, 0xea, 0xd2,
    //         0x52, 0x2c, 0x6b, 0x7b}
    // };
    // std::cout << std::hex;
    // auto routine = AES::key_extension(key);
    // int i = 0;
    // for (auto& w : routine) {
    //     std::cout << "word" << i << ": ";
    //     for (auto& b : w) {
    //         std::cout << (int) b;
    //     }
    //     std::cout << '\n';
    //     ++i;
    // }

    // 54 68 61 74 73 20 6D 79 20 4B 75 6E 67 20 46 75
    auto key = AES::Key<AES::Mode::AES_128>{
        .raw = {
            0x54, 0x68, 0x61, 0x74,
            0x73, 0x20, 0x6d, 0x79,
            0x20, 0x4b, 0x75, 0x6e,
            0x67, 0x20, 0x46, 0x75}
    };
    // 54 77 6F 20 4F 6E 65 20 4E 69 6E 65 20 54 77 6F
    AES::Block in {
        {{0x54, 0x4f, 0x4e, 0x20},
        {0x77, 0x6e, 0x69, 0x54},
        {0x6f, 0x65, 0x6e, 0x77},
        {0x20, 0x20, 0x65, 0x6f}}
    };
    std::cout << std::hex;
    auto result = AES::cipher(in, key);

    std::cout << "Expected: 29 C3 50 5F 57 14 20 F6 40 22 99 B3 1A 02 D7 3A\n";
    std::cout << "Actual: " << std::hex;
    for (auto& row: result) {
        std::cout << row;
    }

    return 0;
}
