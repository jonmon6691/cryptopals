#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "crypto_tools.h"

char *base64map = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define pd(s, d) printf("%s:%d ", s, d)

uint8_t is_wordchar(char a)
{
    return (a >= 'a' && a <= 'z' || a >= 'A' && a <= 'Z' || a == ' ' || a == '\'');
}


float plaintext_score(uint8_t *potential_plaintext, size_t input_length)
{
    float score = 0;
    for (int i=0; i< input_length; i++) {
        score += is_wordchar(potential_plaintext[i])?1.0f:0.0f;
    }
    return score / input_length;
}


struct byte_xor_chunk byte_xor_crack(char* cyphertext, size_t len) {
    struct byte_xor_chunk out;
    out.plaintext = malloc(len);
    out.score = 0;
    out.key = 0;
    out.pt_size = len;
    for (int key = 0; key <= 0xFF; key++) {
        for (int i=0; i < len; i++) {
            out.plaintext[i] = cyphertext[i] ^ key;
        }
        float score = plaintext_score(out.plaintext, len);
        if (score > out.score) {
            out.score = score;
            out.key = key;
        }
    }
    for (int i=0; i<out.pt_size; i++)
        out.plaintext[i] = cyphertext[i] ^ out.key;

    return out;
}


size_t str2bytes(char* input, char **output) {
    unsigned long inputsize = strlen(input);
    /* Note that this will round down, so if an odd number of nibbles
        is input, then the last character will be ignored.*/
    size_t output_size = sizeof(uint8_t) * inputsize / 2;
    char *buff = *output = malloc(output_size);

    for(int i=0, j=0; i < inputsize; i+=2)
        sscanf(&input[i],"%02hhx", &buff[j++]);
    return output_size;
}


size_t bytes2base64(char *input, char **output, size_t input_length)
{
    // 0b12345678   90123456  78901234 Bytes (8 bits each)
    // 0b123456-78 9012-3456 78-901234 Base 64 digits (6 bits each)
    size_t output_size = sizeof(char) * input_length / 3 * 4;
    if (input_length % 3 != 0) // Padding needed, add bytes to the output size
        output_size += 4;
    char *o = *output = malloc(output_size + 1); // Add room for null terminator
    for (int i=0; i < input_length; i+=3) {
        int j = i*4/3;
        o[j+0] = base64map[0x3f & (input[i] >> 2)];
        
        if (i+1 < input_length) {
            o[j+1] = base64map[(0x30 & input[i]<<4) | (0x0f & input[i+1]>>4)];
        } else {
            o[j+1] = base64map[0x30 & input[i]<<4];
            o[j+2] = '=';
            o[j+3] = '=';
            break;
        }
        if (i+2 < input_length) {
            o[j+2] = base64map[(0x3C & input[i+1] << 2) | (0x03 & input[i+2] >> 6)];
            o[j+3] = base64map[(0x3f & input[i+2])];
        } else {
            o[j+2] = base64map[0x3C & input[i+1] << 2];
            o[j+3] = '=';
        }
    }
    o[output_size] = '\0';
    return output_size;
}
