# Advanced Encryption Standard

## Overview

AES (Advanced Encryption Standard) is a block cipher algorithm that operates on 
various key lengths such as 128, 192, and 256 bits. It uses a 
substitution-permutation network with multiple rounds of encryption and 
decryption, depending on key length. It has become the industry 
standard for symmetric encryption due to its security, efficiency, and 
flexibility in supporting different key sizes.

## Implementation

This implementation uses only standard CXX library and GTest for testing
purposes. Project is structured to separate utility functions from cipher and
decipher algorithms to put emphasis on actual AES-related code.

## Build

Using general *build.sh* script is recommended, although its functionality
may be presented in three following steps:

- pull deps using conan with specific system profile
- configure targets
- build targets

Possible commands for that:

```
mkdir build && cd build
conan install .. --output-folder="$PWD/build" --build=missing
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
```

Then executable with CLI is located in *build/src*.
