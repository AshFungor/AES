#pragma once

#include <definitions.hpp>

// standard
#include <stdexcept>
#include <string>
#include <cstdint>
#include <ostream>

namespace text {
    Block parseText(const std::string& input);
    
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