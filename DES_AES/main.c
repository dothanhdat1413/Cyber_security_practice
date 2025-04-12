#include <stdio.h>
#include <stdint.h>
#include "./lib/des/des.h"
#include "./lib/aes/aes.h"

int main() {
    const uint8_t des_key[8] = "myDESkey";  // 64-bit key
    uint8_t aes_key[16] = {
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88,
        0x09, 0xcf, 0x4f, 0x3c
    };

    des_encrypt_file("./input/plaintext.txt", "./output/encrypted_des.txt", des_key);
    des_decrypt_file("./output/encrypted_des.txt", "./output/decrypted_des.txt", des_key);

    printf("DES encryption and decryption completed.\n");

    aes_encrypt_file("./input/plaintext.txt", "./output/encrypted_aes.txt", aes_key);
    aes_decrypt_file("./output/encrypted_aes.txt", "./output/decrypted_aes.txt", aes_key);


    printf("AES encryption and decryption completed.\n");
    return 0;
}
