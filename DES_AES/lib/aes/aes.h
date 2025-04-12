#ifndef AES_H
#define AES_H

#include <stdint.h>

void aes_encrypt_file(const char *infile, const char *outfile, const uint8_t key[16]);
void aes_decrypt_file(const char *infile, const char *outfile, const uint8_t key[16]);

#endif
