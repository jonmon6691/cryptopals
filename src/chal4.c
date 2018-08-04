/*
Detect single-character XOR
One of the 60-character strings in this file has been encrypted by single-character XOR.

Find it.

(Your code from #3 should help.)
*/
/* Detect single-character XOR
    
    One of the 60-character strings in this file has been encrypted by single-character XOR.

    Find it.

    (Your code from #3 should help.)

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto_tools.h"


int main() 
{
    FILE *fp;
    if ((fp = fopen("4.txt", "r")) == NULL) {
        fprintf(stderr, "ERROR: Coudln't open file!");
        exit(1);
    }

    char *cyphertext = malloc(sizeof(char) * 256);
    char *ct_bytes;

    int best_row = 0;
    struct byte_xor_chunk best = {NULL, 0, 0, 0};

    // Loop through the whole file
    for (int row=0; !feof(fp); row++) {
        // Read line from file
        if (fgets(cyphertext, 255, fp) == NULL) {
            fprintf(stderr, "ERROR: Couldn't read file!");
            exit(1);
        }

        // Strip newline
        cyphertext[strnlen(cyphertext, 256)-1] = 0;
        // Convert hex to raw bytes
        size_t ct_size = str2bytes(cyphertext, &ct_bytes);
        // Get the best key
        struct byte_xor_chunk cur = byte_xor_crack(ct_bytes, ct_size);

        // Keep track of the best score so far
        if (cur.score > best.score) {
            best = cur;
            best_row = row + 1;
        } else {
            // Throw away results that aren't the best
            // Dont forget this allocated memory!
            free(cur.plaintext);
        }
        free(ct_bytes);
    }

    printf("Line %d, Key 0x%02x, Score %f: ", best_row, best.key, best.score);
    putc('\'', stdout);
    for (int i=0; i<best.pt_size; i++) putc(best.plaintext[i], stdout);
    putc('\'', stdout);

    free(cyphertext);
    return 0;
}
