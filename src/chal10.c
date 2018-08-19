/* Implement CBC mode
    CBC mode is a block cipher mode that allows us to encrypt
    irregularly-sized messages, despite the fact that a block 
    cipher natively only transforms individual blocks.

    In CBC mode, each ciphertext block is added to the next plaintext 
    block before the next call to the cipher core.

    The first plaintext block, which has no associated previous ciphertext 
    block, is added to a "fake 0th ciphertext block" called the initialization 
    vector, or IV.

    Implement CBC mode by hand by taking the ECB function you wrote earlier, 
    making it encrypt instead of decrypt (verify this by decrypting whatever 
    you encrypt to test), and using your XOR function from the previous exercise 
    to combine them.

    The file here is intelligible (somewhat) when CBC decrypted against 
    "YELLOW SUBMARINE" with an IV of all ASCII 0 (\x00\x00\x00 &c)

    Don't cheat.
    Do not use OpenSSL's CBC code to do CBC mode, even to verify your results. 
    What's the point of even doing this stuff if you aren't going to learn from it?

*/

#include <stdio.h>
#include <string.h>
#include "crypto_tools.h"
#include "aes.h"

int main() 
{
    char message[] = "YELLOW SUBMARINE";
    size_t pt_len;
    char* ciphertext = bas64_decode_file(stdin, &pt_len);
    char IV[16] = {0};
    char* plaintext = malloc(pt_len);

    aes_128_block_decrypt(&ciphertext[0], &plaintext[0], "YELLOW SUBMARINE");
    for (int i = 0; i < 16; i++)
        plaintext[i] ^= IV[i];
    for (int i = 16; i < pt_len; i += 16) {
        aes_128_block_decrypt(&ciphertext[i], &plaintext[i], "YELLOW SUBMARINE");
        plaintext[i] ^= plaintext[i - 16];
    }
    for (int i = 0; i < pt_len; i++)
        printf("%c", plaintext[i]);
    free(plaintext);
    return 0;
}
