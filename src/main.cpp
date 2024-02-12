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

    auto key = AES::Key<128>{
        .mode = AES::Key<128>::Mode::AES_128,
        .raw = {
            0x00, 0x01, 0x02, 0x03,
            0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x10, 0x11,
            0x12, 0x13, 0x14, 0x15}
    };

    AES::Block in {
        {{0x00, 0x01, 0x02, 0x03},
        {0x04, 0x05, 0x06, 0x07},
        {0x08, 0x09, 0x10, 0x11},
        {0x12, 0x13, 0x14, 0x15}}
    };

    auto out = AES::cipher(in, key);

    for (auto& row: out) {
        for (auto& el : row) {
            std::cout << (int) el << ' ';
        }
        std::cout << '\n';
    }
    
    return 0;
}