#include <stdio.h>
#include <stdlib.h>
#include "crypto_tools.h"


char *cyphertext = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
uint8_t *ct_bytes;

#define MIN(a,b) ((a<b)?a:b)

uint8_t is_wordchar(char a)
{
    return (a >= 'a' && a <= 'z' || a >= 'A' && a <= 'Z');

}

float plaintext_score(uint8_t *potential_plaintext, size_t input_length)
{
    float score = 0;
    for (int i=0; i< input_length; i++) {
        score += is_wordchar(potential_plaintext[i])?1:0;
    }
    return score / input_length;
}

int main() 
{
    size_t ct_size = str2bytes(cyphertext, &ct_bytes);
    char *plaintext = (char*)malloc(ct_size+1);
    plaintext[ct_size] = 0; // Null terminated string
    float best_score = 0;

    for (int key = 0; key < 0x100; key++) {
        for (int i=0; i < ct_size; i++) {
            plaintext[i] = ct_bytes[i] ^ key;
        }
        float score = plaintext_score(plaintext, ct_size);
        if (score > best_score) {
            best_score = score;
            printf("0x%02x: %s\n", key, plaintext);
        }
    }

    free(ct_bytes);
    return 0;
}
