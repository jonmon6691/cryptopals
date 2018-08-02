#ifndef CRYPTO_TOOLS_H
#define CRYPTO_TOOLS_H
#include <inttypes.h>

#define MIN(a,b) ((a<b)?a:b)

/* Return 1 if argument is an ASCII letter [a-zA-Z] */
uint8_t is_wordchar(char a);

/* Returns the fraction of bytes in the input that are letters [a-zA-Z] 
    output is a number between 0 and 1, where 1 is all letters */
float plaintext_score(uint8_t *potential_plaintext, size_t input_length);

/* Converts a single hex character into an integer in the range [0-15] */
uint8_t hexdig2int(char dig);

/* Input bust be a null-terminated string of an even number of hex
    characters, case-insensitive. Allocates a buffer for the bytes
    and stores the pointer in output, returns the size of the output buffer */
size_t str2bytes(char* input, char **output);

/* Allocates a buffer for the base26 output string, null terminated,
    stores the pointer in output and returns the size of the 
    output buffer. */
size_t bytes2base64(char *input, char **output, size_t input_length);

#endif
