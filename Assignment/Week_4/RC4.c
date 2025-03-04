#include <stdio.h>

#define DAT_OK 1

#define ORIGIN_LEN 10
#define SEED_LEN 4
#define PLAIN_TEXT_LEN 6

int S[ORIGIN_LEN];
int T[ORIGIN_LEN];
int Seed[SEED_LEN] = {1,2,3,6};
int K[PLAIN_TEXT_LEN];

int swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    return DAT_OK;
}

int init_T(int* T, int origin_len, int* Seed, int seed_len){
    for(int i = 0; i < origin_len; i++) {
        T[i] = Seed[i%seed_len];
    }
    return DAT_OK;
}

int init_S(int* T, int* S, int origin_len){
    for (int i = 0; i < origin_len; i++){
        S[i] = i;
    }
    int j = 0;
    for (int i = 0; i < origin_len; i++){
        j = (j + S[i] + T[i])% origin_len;
        swap(S+i, S+j);
    }
    return DAT_OK;
}

int gen_K(int* K, int plain_text_len, int* S, int origin_len) {
    int j = 0;
    int t = 0;
    for(int i = 1; i < plain_text_len+1; i++){
        j = (j + S[i])%origin_len;
        swap(S+i,S+j);
        t = (S[i] + S[j])%origin_len;
        K[i-1] = S[t];
    }
    return DAT_OK;
}

int RC4_run(int* T, int* S, int* Seed, int origin_len, int seed_len, int plain_text_len){
    if(init_T(T,origin_len, Seed, seed_len) != DAT_OK){
        printf("init T failed");
    }
    for(int i = 0; i < origin_len; i++){
        printf("%d",T[i]);
    }
    printf("\n");

    if(init_S(T,S,origin_len) != DAT_OK){
        printf("init S failed");
    }
    for(int i = 0; i < origin_len; i++){
        printf("%d",S[i]);
    }
    printf("\n");

    if(gen_K(K,plain_text_len,S,origin_len) != DAT_OK){
        printf("gen Key failed");
    }

    for(int i = plain_text_len -1 ; i >= 0; i--){
        printf("%d",K[i]);
    }
    printf("\n");
    
    return DAT_OK;
}
int main(){
    
    if( RC4_run(T,S,Seed,ORIGIN_LEN,SEED_LEN,PLAIN_TEXT_LEN) != DAT_OK){
        printf("RC_4 run failed");
    } // in theo dạng K[N] -> K[0]
    return 0;
}