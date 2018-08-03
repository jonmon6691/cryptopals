#include <stdio.h>
#include <stdlib.h>
#include "crypto_tools.h"


char *cyphertext = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
uint8_t *ct_bytes;


int main() 
{
    size_t ct_size = str2bytes(cyphertext, &ct_bytes);
    
    struct byte_xor_chunk key = byte_xor_crack(ct_bytes, ct_size);

    printf("%02x: ", key.key);
    for (int i=0; i < key.pt_size; i++)
        putc(key.plaintext[i], stdout);
    putc('\n', stdout);
    
    free(key.plaintext);
    free(ct_bytes);
    return 0;
}
