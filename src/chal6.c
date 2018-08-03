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
