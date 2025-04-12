#ifndef DES_H
#define DES_H

#include <stdint.h>

void des_encrypt_file(const char *infile, const char *outfile, const uint8_t key[8]);
void des_decrypt_file(const char *infile, const char *outfile, const uint8_t key[8]);

#endif
