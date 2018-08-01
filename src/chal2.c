#include <stdio.h>
#include "crypto_tools.h"


char *input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
char *input_bytes;


int main() 
{
    size_t input_size = str2bytes(input, &input_bytes);
    char *b64;
    size_t b64_size = bytes2base64(input_bytes, &b64, input_size);
    for (int i=0; i < b64_size; i++)
        printf("%c", b64[i]);
    printf("\n%s", b64);

    free(b64);
    free(input_bytes);
    return 0;
}
