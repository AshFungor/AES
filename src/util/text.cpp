#include <definitions.hpp>

// standard
#include <algorithm>

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