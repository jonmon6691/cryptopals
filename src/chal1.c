#include <stdio.h>
#include "crypto_tools.h"


char *input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
char *input_bytes;
char *b64_output;


int main() 
{
    size_t input_size = str2bytes(input, &input_bytes);
    size_t b64_size = bytes2base64(input_bytes, &b64_output, input_size);

    printf("%s\n", b64_output);

    free(b64_output);
    free(input_bytes);
    return 0;
}
