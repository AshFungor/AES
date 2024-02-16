// AES
#include <aes.hpp>

// util
#include <ios>
#include <text.hpp>
#include <numeric.hpp>
#include <definitions.hpp>

// standard
#include <iostream>
#include <fstream>
#include <iomanip>
#include <utility>
#include <any>

template<Mode BitMode>
void encrypt() {
    std::string msg;
    std::cout << "Enter your message:\n"; 
    std::getline(std::cin >> std::ws, msg); 

    std::string textKey;
    std::cout << "Enter key:\n"; 
    std::getline(std::cin >> std::ws, textKey);

    Key<BitMode> binaryKey = text::parseKey<BitMode>(textKey);
    std::ofstream ofs {"out.bin", std::ios::binary};
    for (const auto& block : text::splitText(msg)) {
        ofs << text::join(AES::cipher(block, binaryKey));
    }
    std::cout << "Message ready. Encrypted text is written to file: out.bin\n";
}

template<Mode BitMode>
void decrypt() {
    std::string path;
    std::cout << "Input path to file with binary data: "; std::cin >> path; 

    std::string textKey;
    std::cout << "Enter key: "; std::getline(std::cin >> std::ws, textKey);

    Key<BitMode> binaryKey = text::parseKey<BitMode>(textKey);
    std::vector<Block> resultData;
    for (const auto& block : text::splitBinary(path)) {
        resultData.push_back(AES::decipher(block, binaryKey));
    }
    std::cout << "Message ready: " << text::parseBinary(resultData) << '\n';
}

int main() {
    
    std::cout << "This is AES encryption/decryption program. First, select key mode:\n"
              << "1) 128 bit key (16 letters in password)\n"
              << "2) 192 bit key (24 letters in password)\n"
              << "3) 256 bit key (32 letters in password)\n";
    int mode; std::cin >> mode;

    std::cout << "What do you want to do? (1 - encrypt, 2 - decrypt)\n";
    int choice; std::cin >> choice;
    
    switch (choice) {
    case 1:
        switch (mode) {
        case 1:
            encrypt<Mode::AES_128>();
            return 0;
        case 2:
            encrypt<Mode::AES_192>();
            return 0;
        case 3:
            encrypt<Mode::AES_256>();
            return 0;
        default:
            std::cout << "Error: unknown mode!";
            return 1;
        }
    case 2:
        switch (mode) {
        case 1:
            decrypt<Mode::AES_128>();
            return 0;
        case 2:
            decrypt<Mode::AES_192>();
            return 0;
        case 3:
            decrypt<Mode::AES_256>();
            return 0;
        default:
            std::cout << "Error: unknown mode!";
            return 1;
        }
    default:
        std::cout << "Error: unknown intent!";
    }
    return 1;
}
