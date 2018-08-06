/* Detect AES in ECB mode
    In this file are a bunch of hex-encoded ciphertexts.

    One of them has been encrypted with ECB.

    Detect it.

    Remember that the problem with ECB is that it is stateless and deterministic; the same 16 byte plaintext block will always produce the same 16 byte ciphertext.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto_tools.h"

#define CHUNK_SIZE 1024
char file_buf[CHUNK_SIZE];

char *key = "ICE";
size_t key_len = 3;

int main() 
{
    // Loop through the whole file
    int key_index = 0;
    while (!feof(stdin)) {
        size_t in_len = fread(file_buf, sizeof(char), CHUNK_SIZE, stdin);
        for (int i=0; i<in_len; i++)
            printf("%02x", file_buf[i] ^ key[key_index++%key_len]);
    }
    return 0;
}
