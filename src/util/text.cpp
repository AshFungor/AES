#include <definitions.hpp>

// standard
#include <cctype>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Local
#include "text.hpp"

std::vector<Block> text::splitBinary(const std::string& filename) {
    std::vector<Block> result;
    std::ifstream ifs {filename, std::ios::binary};
    char inByte;

    while (ifs.peek() != EOF) {
        result.push_back({});
        for (int i = 0; i < Nb * 4; ++i) {
            if (!(ifs.read(&inByte, 1))) inByte = ' ';
            result.back()[i % 4][i / 4] = reinterpret_cast<byte&>(inByte);
        }
    }
    return result;
}

std::string text::generateKey(int size) {
    std::random_device rd; 
    auto ss = std::seed_seq {
        rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()
    };
    std::mt19937 rand {ss};
    std::string result;
    result.resize(size);
    while (size--) {
        byte symbol = rand() % 256;
        while (!(std::isalpha(symbol) || std::isdigit(symbol))) {
            symbol = rand() % 256;
        }
        result[size] = symbol;
    }
    return result;
}

std::string text::join(Block data) {
    std::stringstream result;
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < Nb; ++column) {
            result << reinterpret_cast<char&>(data[column][row]);
        }
    }
    return result.str();
}