// AES
#include <aes.hpp>

// util
#include <text.hpp>
#include <numeric.hpp>
#include <definitions.hpp>

// standard
#include <random>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <utility>
#include <ios>
#include <any>

template<Mode BitMode>
void encrypt() {
    std::string path;
    std::cout << "Input path to file with text data:\n"; 
    std::cin >> path; 


    std::cout << "Use key autogen? (0 - no, 1 - yes)\n";
    int autogen; std::cin >> autogen;
    std::string textKey;
    if (!autogen) {
        std::cout << "Enter key:\n"; 
        std::cin >> textKey;
    } else {
        textKey = text::generateKey((int) BitMode / 8);
        std::cout << "Your key is: " << textKey << '\n';
    }

    Key<BitMode> binaryKey = text::parseKey<BitMode>(textKey);
    std::ofstream ofs {"out.bin", std::ios::binary};
    for (const auto& block : text::splitBinary(path)) {
        ofs << text::join(AES::cipher(block, binaryKey));
    }
    std::cout << "Message ready. Encrypted text is written to file: out.bin\n";
}

template<Mode BitMode>
void decrypt() {
    std::string path;
    std::cout << "Input path to file with binary data:\n"; 
    std::cin >> path; 

    std::string textKey;
    std::cout << "Enter key:\n"; 
    std::cin >> textKey;

    Key<BitMode> binaryKey = text::parseKey<BitMode>(textKey);
    std::ofstream ofs {"out.txt", std::ios::binary};
    for (const auto& block : text::splitBinary(path)) {
        ofs << text::join(AES::decipher(block, binaryKey));
    }
    std::cout << "Message ready. Decrypted text is written to file: out.txt\n";
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
