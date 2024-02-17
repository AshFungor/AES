#pragma once

#include <definitions.hpp>

// standard
#include <stdexcept>
#include <string>
#include <cstdint>
#include <ostream>
#include <vector>

namespace text {
    std::vector<Block> splitBinary(const std::string& filename);
    std::string generateKey(int size);
    std::string join(Block data);
    
    template<Mode BitMode>
    Key<BitMode> parseKey(const std::string& input);
} // namespace test

template<Mode BitMode>
Key<BitMode> text::parseKey(const std::string& input) {
    if (input.size() != (int) BitMode / 8) {
        throw std::runtime_error(
            "size of input string is " + std::to_string(input.size()) + ", expected: " + std::to_string((int) BitMode / 8));
    }

    Key<BitMode> result;
    for (int i = 0; i < input.size(); ++i) {
        result.raw[i] = reinterpret_cast<const byte&>(input[i]);
    }

    return result;
}