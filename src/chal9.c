/* Implement PKCS#7 padding
    A block cipher transforms a fixed-sized block (usually 8 or 16 bytes) of plaintext into ciphertext. But we almost never want to transform a single block; we encrypt irregularly-sized messages.

    One way we account for irregularly-sized messages is by padding, creating a plaintext that is an even multiple of the blocksize. The most popular padding scheme is called PKCS#7.

    So: pad any block to a specific block length, by appending the number of bytes of padding to the end of the block. For instance,

    "YELLOW SUBMARINE"
    ... padded to 20 bytes would be:

    "YELLOW SUBMARINE\x04\x04\x04\x04"
    
*/

#include <stdio.h>
#include <string.h>

#define CHUNK_SIZE 1024
char file_buf[CHUNK_SIZE];

// AES 128 bit block represented in hex takes 32 bytes 
#define BLOCK_LEN 32

int main() 
{
    // Loop through the whole file
    for (int line = 1; !feof(stdin); line++) {
        // Line by line
        fgets(file_buf, CHUNK_SIZE, stdin);
        // Get the length of the line read in
        size_t in_len = strnlen(file_buf, CHUNK_SIZE);
        // Loop through all the encrypted blocks
        for (int i = 0; i < in_len; i += BLOCK_LEN)
            // Loop through the remaining of the encrypted blocks
            for (int j = i + BLOCK_LEN; j < in_len; j += BLOCK_LEN)
                // Compare the two, print if a match is found
                if (memcmp(&file_buf[i], &file_buf[j], BLOCK_LEN) == 0)
                    printf("Row %d, Blocks %d, %d both %.32s\n", line, i/BLOCK_LEN, j/BLOCK_LEN, &file_buf[i]);
    }
    return 0;
}
