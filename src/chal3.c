#include <stdio.h>
#include <stdlib.h>
#include "crypto_tools.h"


char *cyphertext = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
uint8_t *ct_bytes;


int main() 
{
    size_t ct_size = str2bytes(cyphertext, &ct_bytes);
    
    uint8_t key = byte_xor_crack(ct_bytes, ct_size);

    printf("%02x: ", key);
    for (int i=0; i < ct_size; i++)
        putc(ct_bytes[i] ^ key, stdout);
    putc('\n', stdout);
    
    free(ct_bytes);
    return 0;
}
