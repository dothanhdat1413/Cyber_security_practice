#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

char keyTable[SIZE][SIZE];

// Hàm kiểm tra sự tồn tại của ký tự trong bảng khóa
int isCharInTable(char ch, char keyTable[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (keyTable[i][j] == ch) {
                return 1;
            }
        }
    }
    return 0;
}

// Hàm tạo bảng khóa từ keyword
void generateKeyTable(const char *key) {
    int used[26] = {0};  // Đánh dấu ký tự đã dùng
    int i, j, k = 0;
    char filteredKey[26];
    
    // Xử lý key: Loại bỏ trùng, thay 'J' bằng 'I'
    int len = 0;
    for (i = 0; key[i] != '\0'; i++) {
        char ch = toupper(key[i]);
        if (ch == 'J') ch = 'I';  // Thay J bằng I
        if (!used[ch - 'A'] && ch >= 'A' && ch <= 'Z') {
            used[ch - 'A'] = 1;
            filteredKey[len++] = ch;
        }
    }

    // Điền key vào bảng khóa
    i = j = 0;
    for (k = 0; k < len; k++) {
        keyTable[i][j++] = filteredKey[k];
        if (j == SIZE) { i++; j = 0; }
    }

    // Điền nốt bảng với các chữ cái chưa xuất hiện
    for (k = 0; k < 26; k++) {
        char ch = 'A' + k;
        if (ch == 'J') continue;  // Bỏ qua J (đã gộp vào I)
        if (!used[k]) {
            keyTable[i][j++] = ch;
            if (j == SIZE) { i++; j = 0; }
        }
    }
}

// Hàm tìm vị trí của một ký tự trong bảng khóa
void findPosition(char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I';  // Gộp J vào I
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (keyTable[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Tiền xử lý văn bản (loại bỏ khoảng trắng, chèn X nếu cần)
void preprocessText(const char *text, char *processedText) {
    int len = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            processedText[len++] = toupper(text[i]);
        }
    }
    processedText[len] = '\0';

    // Chèn X nếu cần để tạo cặp
    char finalText[100];
    int finalLen = 0;
    for (int i = 0; i < len; i++) {
        finalText[finalLen++] = processedText[i];
        if (i + 1 < len && processedText[i] == processedText[i + 1]) {
            finalText[finalLen++] = 'X'; // Tránh lặp ký tự trong cặp
        }
    }
    if (finalLen % 2 != 0) {
        finalText[finalLen++] = 'X'; // Thêm X nếu chuỗi lẻ ký tự
    }
    finalText[finalLen] = '\0';

    strcpy(processedText, finalText);
}

// Hàm mã hóa văn bản theo thuật toán Playfair
void encrypt(const char *text, char *cipherText) {
    char processedText[100];
    preprocessText(text, processedText);

    int len = strlen(processedText);
    int row1, col1, row2, col2;

    for (int i = 0; i < len; i += 2) {
        findPosition(processedText[i], &row1, &col1);
        findPosition(processedText[i + 1], &row2, &col2);

        if (row1 == row2) {  // Cùng hàng
            cipherText[i] = keyTable[row1][(col1 + 1) % SIZE];
            cipherText[i + 1] = keyTable[row2][(col2 + 1) % SIZE];
        } else if (col1 == col2) {  // Cùng cột
            cipherText[i] = keyTable[(row1 + 1) % SIZE][col1];
            cipherText[i + 1] = keyTable[(row2 + 1) % SIZE][col2];
        } else {  // Khác hàng, khác cột
            cipherText[i] = keyTable[row1][col2];
            cipherText[i + 1] = keyTable[row2][col1];
        }
    }
    cipherText[len] = '\0';
}

// Hàm in bảng khóa
void printKeyTable() {
    printf("Playfair Key Table:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", keyTable[i][j]);
        }
        printf("\n");
    }
}

// Hàm main để kiểm tra chương trình
int main() {
    char key[100], text[100], cipherText[100];

    // Nhập khóa và văn bản
    printf("Key: ");
    scanf("%s", key);
    printf("Plain text: ");
    scanf("%s", text);

    // Tạo bảng khóa và mã hóa
    generateKeyTable(key);
    printKeyTable();
    encrypt(text, cipherText);

    // Xuất kết quả
    printf("Encrypted text: %s\n", cipherText);
    return 0;
}
