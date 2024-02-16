#include <definitions.hpp>

// standard
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Local
#include <text.hpp>

Block text::parseText(const std::string& input) {
    Block result;
    for (int i = 0; i < std::max(input.size(), 4 * Nb); ++i) {
        if (i >= input.size()) {
            break;
        }
        result[i / 4][i % 4] = reinterpret_cast<const byte&>(input[i]);
    }
    return result;
}

std::vector<Block> text::splitText(const std::string& input) {
    std::size_t iter = 0;
    std::vector<Block> result;

    auto parse = [&](const std::string& in) {
        result.push_back({});
        for (std::size_t start = iter; iter - start < Nb * 4; ++iter) {
            result.back()[(iter - start) / 4][(iter - start) % 4] = reinterpret_cast<const byte&>(in[iter]);
        }
    };

    while (iter + Nb * 4 < input.size()) {
        parse(input);
    }

    std::string padded = input.substr(iter); 
    while (input.size() < iter + Nb * 4) {
        padded.append("\0");
    }

    iter = 0;
    parse(padded);
    return result;
}

std::string text::parseBinary(const std::vector<Block>& data) {
    std::string result;
    for (const auto& block : data) {
        result.append(text::join(block));
    }
    return result;
}

std::vector<Block> text::splitBinary(const std::string& filename) {
    std::ifstream ifs {filename, std::ios::binary};
    std::vector<Block> result;
    byte inByte;

    while (!ifs.eof()) {
        result.push_back({});
        for (int i = 0; i < Nb * 4; ++i) {
            ifs >> inByte;
            result.back()[i / 4][i % 4] = inByte;
        }
    }

    return result;
}

std::string text::join(Block data) {
    std::stringstream result;
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < Nb; ++column) {
            result << data[column][row];
        }
    }
    return result.str();
}