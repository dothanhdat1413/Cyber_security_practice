# DES and AES Encryption Implementation

This midterm project implements the Data Encryption Standard (DES) and Advanced Encryption Standard (AES) algorithms for file encryption and decryption. It provides a simple command-line interface to encrypt and decrypt files using both algorithms. 

## Features

- DES encryption and decryption (64-bit block cipher with 56-bit key)
- AES-128 encryption and decryption (128-bit block cipher with 128-bit key)
- File-based operations with automatic padding
- Support for text files of any size

## Project Structure
```
.
├── main.c                 # Main program file
├── Makefile               # Build configuration
├── Readme.md              # This file
├── build/                 # Compiled binary output directory
├── input/                 # Input files directory
│   └── plaintext.txt      # Sample plaintext to encrypt
├── lib/                   # Library code
│   ├── aes/               # AES implementation
│   │   ├── aes.c
│   │   └── aes.h
│   └── des/               # DES implementation
│       ├── des.c
│       └── des.h
└── output/                # Encrypted and decrypted output files directory
```

## Building and Running

### Prerequisites

- GCC compiler
- Make utility

### Build Instructions

To compile the program:

```
make
```

This will build the executable in the build directory.

Running the Program
To run the program:

```
make run
```

The program will:

- Encrypt input/plaintext.txt using DES to output/encrypted_des.txt
- Decrypt the DES-encrypted file to output/decrypted_des.txt
- Encrypt input/plaintext.txt using AES to output/encrypted_aes.txt
- Decrypt the AES-encrypted file to output/decrypted_aes.txt

Clean Build Files
To clean up build artifacts and output files:

```
make clean
```

## Implementation Details
### DES Implementation
1. Uses the standard 16-round Feistel network structure   
2. Implements all the required permutation tables (IP, FP, E, P, S-boxes)   
3. Includes key scheduling with 56-bit effective key size   
4. Implements PKCS#7 padding for input files   
### AES Implementation
1. Implements the standard 10-round AES-128 algorithm  
2. Features all four main operations: SubBytes, ShiftRows, MixColumns, and AddRoundKey   
3. Includes key expansion for generating round keys   
4. Uses PKCS#7 padding for input files   
## Customizing
To use different keys, modify the main.c file:

```
// Change DES key (8 bytes)
const uint8_t des_key[8] = "myDESkey";

// Change AES key (16 bytes)
uint8_t aes_key[16] = {
    0x2b, 0x7e, 0x15, 0x16,
    0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88,
    0x09, 0xcf, 0x4f, 0x3c
};

```

To change different plaintext, modify the input/plaintext.txt