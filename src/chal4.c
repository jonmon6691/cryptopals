/*
Detect single-character XOR
One of the 60-character strings in this file has been encrypted by single-character XOR.

Find it.

(Your code from #3 should help.)
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto_tools.h"

int main() 
{
    FILE *fp;
    if ((fp = fopen("4.txt", "r")) == NULL) {
        fprintf(stderr, "ERROR: Coudln't open file!");
        exit(1);
    }

    char *plaintext = malloc(sizeof(char) * 256);
    char *cyphertext = malloc(sizeof(char) * 256);
    char *ct_bytes;

    float best_score = 0;
    uint8_t best_key;
    int best_row = 0;
    char *best_plaintext = malloc(sizeof(char) * 128);

    int row = 0;
    while (!feof(fp)) {
        if (fgets(cyphertext, 255, fp) == NULL) {
            fprintf(stderr, "ERROR: Couldn't read file!");
            exit(1);
        }

        cyphertext[strnlen(cyphertext, 256)-1] = 0; // Strip newline
        printf("%s ", cyphertext);
        size_t ct_size = str2bytes(cyphertext, &ct_bytes);

        uint8_t key = byte_xor_crack(ct_bytes, ct_size);
        printf("%02x,", key);

        for (int i=0; i<ct_size; i++) { // Decrypt in place
            //ct_bytes[i] ^= key;
            putc(ct_bytes[i] ^ key, stdout);
        }

        float score = plaintext_score(ct_bytes, ct_size);
        printf("%f\n", score);
        if (score > best_score) {
            best_score = score;
            best_key = key;
            best_row = row;
            for (int i=0; i<ct_size; i++)
                best_plaintext[i] = ct_bytes[i];
        }
        free(ct_bytes);
        row++;
    }

    printf("Line %d, Key %02x: ", best_row, best_key);
    for (int i=0; i<128; i++)
        putc(best_plaintext[i], stdout);
    printf("\n");

    free(cyphertext);
    free(plaintext);
    return 0;
}
