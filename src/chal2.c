/* Fixed XOR
    
    Write a function that takes two equal-length buffers and produces their XOR combination.

    If your function works properly, then when you feed it the string:
    1c0111001f010100061a024b53535009181c
    ... after hex decoding, and when XOR'd against:
    686974207468652062756c6c277320657965
    ... should produce:
    746865206b696420646f6e277420706c6179

*/

#include <stdio.h>
#include "crypto_tools.h"


char *a = "1c0111001f010100061a024b53535009181c";
char *b = "686974207468652062756c6c277320657965";
/* a^b = 0x746865206b696420646f6e277420706c6179 */

uint8_t *a_bytes;
uint8_t *b_bytes;

#define MIN(a,b) ((a<b)?a:b)

int main() 
{
    size_t a_size = str2bytes(a, &a_bytes);
    size_t b_size = str2bytes(b, &b_bytes);

    for (int i=0; i < MIN(a_size,b_size); i++)
        printf("%02x", a_bytes[i]^b_bytes[i]);
    printf("\n");

    free(a_bytes);
    free(b_bytes);
    return 0;
}
