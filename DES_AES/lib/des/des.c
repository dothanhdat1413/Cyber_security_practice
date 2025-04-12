#include "des.h"
#include <stdio.h>
#include <string.h>

#define ROUNDS 16

static const int IP[] = {
 58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,
 62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
 57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
 61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7
};

static const int FP[] = {
 40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,
 38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,
 36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,
 34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25
};

static const int E[] = {
 32,1,2,3,4,5,4,5,6,7,8,9,
 8,9,10,11,12,13,12,13,14,15,16,17,
 16,17,18,19,20,21,20,21,22,23,24,25,
 24,25,26,27,28,29,28,29,30,31,32,1
};

static const int P[] = {
 16,7,20,21,29,12,28,17,1,15,23,26,
 5,18,31,10,2,8,24,14,32,27,3,9,
 19,13,30,6,22,11,4,25
};

static const int S[8][4][16] = {
{
{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
},
{
{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
},
{
{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
},
{
{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
},
{
{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
},
{
{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
},
{
{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
},
{
{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
}
};

static const int PC1[] = {
57,49,41,33,25,17,9,1,58,50,42,34,26,18,
10,2,59,51,43,35,27,19,11,3,60,52,44,36,
63,55,47,39,31,23,15,7,62,54,46,38,30,22,
14,6,61,53,45,37,29,21,13,5,28,20,12,4
};

static const int PC2[] = {
14,17,11,24,1,5,3,28,15,6,21,10,
23,19,12,4,26,8,16,7,27,20,13,2,
41,52,31,37,47,55,30,40,51,45,33,48,
44,49,39,56,34,53,46,42,50,36,29,32
};

static const int shifts[] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

static void permute(const uint8_t *in, uint8_t *out, const int *table, int bits) {
    memset(out, 0, bits/8);
    for (int i = 0; i < bits; i++) {
        int src_byte = (table[i]-1) / 8;
        int src_bit = (table[i]-1) % 8;
        int val = (in[src_byte] >> (7 - src_bit)) & 1;
        out[i/8] |= val << (7 - (i % 8));
    }
}

static void left_rotate28(uint8_t *half, int shifts) {
    uint32_t v = ((uint32_t)half[0] << 20) | ((uint32_t)half[1] << 12) | ((uint32_t)half[2] << 4) | (half[3] >> 4);
    v = ((v << shifts) | (v >> (28 - shifts))) & 0x0FFFFFFF;
    half[0] = (v >> 20) & 0xFF;
    half[1] = (v >> 12) & 0xFF;
    half[2] = (v >> 4) & 0xFF;
    half[3] = (v << 4) & 0xF0;
}

static void generate_subkeys(const uint8_t key[8], uint8_t subkeys[16][6]) {
    uint8_t permuted[7] = {0}, C[4], D[4];
    permute(key, permuted, PC1, 56);
    memcpy(C, permuted, 4);
    memcpy(D, permuted + 3, 4);

    for (int i = 0; i < ROUNDS; i++) {
        left_rotate28(C, shifts[i]);
        left_rotate28(D, shifts[i]);
        uint8_t CD[7] = {0};
        memcpy(CD, C, 4);
        CD[3] |= D[0] >> 4;
        memcpy(CD+4, D+1, 3);
        permute(CD, subkeys[i], PC2, 48);
    }
}

static uint32_t feistel(uint32_t r, const uint8_t subkey[6]) {
    uint8_t expanded[6] = {0};
    uint8_t r_bytes[4] = {
        (r >> 24) & 0xFF, (r >> 16) & 0xFF,
        (r >> 8) & 0xFF, r & 0xFF
    };
    permute(r_bytes, expanded, E, 48);

    for (int i = 0; i < 6; i++) expanded[i] ^= subkey[i];

    uint32_t out = 0;
    for (int i = 0; i < 8; i++) {
        int row = ((expanded[i/6] >> (7 - (i*6)%8)) & 0x2) | ((expanded[i/6] >> (7 - (i*6 + 5)%8)) & 0x1);
        int col = (expanded[i/6] >> (7 - ((i*6+1)%8))) & 0xF;
        out = (out << 4) | S[i][row][col];
    }

    uint8_t out_bytes[4] = {
        (out >> 24) & 0xFF, (out >> 16) & 0xFF,
        (out >> 8) & 0xFF, out & 0xFF
    };
    uint8_t permuted[4] = {0};
    permute(out_bytes, permuted, P, 32);
    return (permuted[0] << 24) | (permuted[1] << 16) | (permuted[2] << 8) | permuted[3];
}

static void des_process_block(uint8_t *block, uint8_t subkeys[16][6], int decrypt) {
    uint8_t ip[8] = {0};
    permute(block, ip, IP, 64);

    uint32_t L = (ip[0]<<24) | (ip[1]<<16) | (ip[2]<<8) | ip[3];
    uint32_t R = (ip[4]<<24) | (ip[5]<<16) | (ip[6]<<8) | ip[7];

    for (int i = 0; i < ROUNDS; i++) {
        uint32_t temp = R;
        int round = decrypt ? (15 - i) : i;
        R = L ^ feistel(R, subkeys[round]);
        L = temp;
    }

    uint32_t pre_output = (R << 32) | L;
    uint8_t before_fp[8] = {
        (R >> 24) & 0xFF, (R >> 16) & 0xFF,
        (R >> 8) & 0xFF, R & 0xFF,
        (L >> 24) & 0xFF, (L >> 16) & 0xFF,
        (L >> 8) & 0xFF, L & 0xFF
    };
    permute(before_fp, block, FP, 64);
}

void des_encrypt_file(const char *infile, const char *outfile, const uint8_t key[8]) {
    FILE *fin = fopen(infile, "rb");
    FILE *fout = fopen(outfile, "wb");
    uint8_t subkeys[16][6];
    generate_subkeys(key, subkeys);

    uint8_t block[8];
    while (1) {
        size_t read = fread(block, 1, 8, fin);
        if (read == 0) break;
        if (read < 8) memset(block + read, 8 - read, 8 - read);
        des_process_block(block, subkeys, 0);
        fwrite(block, 1, 8, fout);
    }

    fclose(fin);
    fclose(fout);
}

// void des_decrypt_file(const char *infile, const char *outfile, const uint8_t key[8]) {
//     FILE *fin = fopen(infile, "rb");
//     FILE *fout = fopen(outfile, "wb");
//     uint8_t subkeys[16][6];
//     generate_subkeys(key, subkeys);

//     uint8_t block[8];
//     while (1) {
//         size_t read = fread(block, 1, 8, fin);
//         if (read == 0) break;
//         des_process_block(block, subkeys, 1);
//         fwrite(block, 1, 8, fout);
//     }

//     fclose(fin);
//     fclose(fout);
// }

void des_decrypt_file(const char *infile, const char *outfile, const uint8_t key[8]) {
    FILE *fin = fopen(infile, "rb");
    FILE *fout = fopen(outfile, "wb");
    uint8_t subkeys[16][6];
    generate_subkeys(key, subkeys);

    uint8_t block[8], next_block[8];
    size_t read, next_read;

    // Đọc block đầu tiên
    read = fread(block, 1, 8, fin);

    while ((next_read = fread(next_block, 1, 8, fin)) == 8) {
        des_process_block(block, subkeys, 1);
        fwrite(block, 1, 8, fout);
        memcpy(block, next_block, 8);
    }

    // Xử lý block cuối cùng
    if (read > 0) {
        des_process_block(block, subkeys, 1);

        // Loại bỏ padding từ block cuối
        uint8_t pad = block[7];
        if (pad > 0 && pad <= 8) {
            fwrite(block, 1, 8 - pad, fout);
        } else {
            fwrite(block, 1, 8, fout);  // padding không hợp lệ → ghi nguyên
        }
    }

    fclose(fin);
    fclose(fout);
}

