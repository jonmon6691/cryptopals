/* Convert hex to base64

    The string:
    49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d
    Should produce:
    SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t

    So go ahead and make that happen. You'll need to use this code for the rest of the exercises.

    Cryptopals Rule
    Always operate on raw bytes, never on encoded strings. Only use hex and base64 for pretty-printing.

*/

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
