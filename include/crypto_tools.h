#ifndef CRYPTO_TOOLS_H
#define CRYPTO_TOOLS_H
#include <inttypes.h>

#define MIN(a,b) ((a<b)?a:b)

struct byte_xor_chunk {
        char *plaintext;
        size_t pt_size;
        uint8_t key;
        float score;
};

/* Returns the hamming distance of two buffers */
int hamming_distance(char *a, char *b, size_t len);

/* Return 1 if argument is an ASCII letter [a-zA-Z] */
uint8_t is_wordchar(char a);

/* Returns the fraction of bytes in the input that are letters [a-zA-Z] 
    output is a number between 0 and 1, where 1 is all letters */
float plaintext_score(uint8_t *potential_plaintext, size_t input_length);

/* Brute forces a 1-byte xor key over an encrypted buffer, finds
    the key that maximizes the plaintext_score of the output.
    Returns a data structure with the result */
struct byte_xor_chunk byte_xor_crack(char* cyphertext, size_t len);

/* Input bust be a null-terminated string of hex characters, case-insensitive.
    Allocates a buffer for the bytes and stores the pointer in output, 
    returns the size of the output buffer. If the input length is odd, the last
    nibble is ignored. */
size_t str2bytes(char* input, char **output);

/* Allocates a buffer for the base26 output string, null terminated,
    stores the pointer in output and returns the size of the 
    output buffer. */
size_t bytes2base64(char *input, char **output, size_t input_length);

#endif
