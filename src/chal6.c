/* Break repeating-key XOR
    
    It is officially on, now.
    This challenge isn't conceptually hard, but it involves actual error-prone coding. The other challenges in this set are there to bring you up to speed. This one is there to qualify you. If you can do this one, you're probably just fine up to Set 6.

    There's a file here. It's been base64'd after being encrypted with repeating-key XOR.

    Decrypt it.

    Here's how:
    Let KEYSIZE be the guessed length of the key; try values from 2 to (say) 40.
    Write a function to compute the edit distance/Hamming distance between two strings.

    The Hamming distance is just the number of differing bits. The distance between:
    this is a test
    and
    wokka wokka!!!
    is 37. Make sure your code agrees before you proceed.
    
    For each KEYSIZE, take the first KEYSIZE worth of bytes, and the second KEYSIZE worth of bytes, and find the edit distance between them. Normalize this result by dividing by KEYSIZE.
    The KEYSIZE with the smallest normalized edit distance is probably the key. You could proceed perhaps with the smallest 2-3 KEYSIZE values. Or take 4 KEYSIZE blocks instead of 2 and average the distances.
    Now that you probably know the KEYSIZE: break the ciphertext into blocks of KEYSIZE length.
    Now transpose the blocks: make a block that is the first byte of every block, and a block that is the second byte of every block, and so on.
    Solve each block as if it was single-character XOR. You already have code to do this.
    For each block, the single-byte XOR key that produces the best looking histogram is the repeating-key XOR key byte for that block. Put them together and you have the key.
    This code is going to turn out to be surprisingly useful later on. Breaking repeating-key XOR ("Vigenere") statistically is obviously an academic exercise, a "Crypto 101" thing. But more people "know how" to break it than can actually break it, and a similar technique breaks something much more important.

    No, that's not a mistake.
    We get more tech support questions for this challenge than any of the other ones. We promise, there aren't any blatant errors in this text. In particular: the "wokka wokka!!!" edit distance really is 37.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/applink.c>
#include "crypto_tools.h"

#define MAX_KEY_SIZE 40

#define CHUNK_SIZE 1024
char file_buf[CHUNK_SIZE];

struct link {
    char *data;
    size_t data_len;
    struct link *next;
};

/* Overkill file loader that loads an arbitrarily large file into
    a contiguous buffer. */
char *load_base64_file(FILE *fp, size_t *data_size)
{
    // Set up OpenSSL base64 pipeline
    BIO *bio, *b64;
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_fp(fp, BIO_NOCLOSE);
    BIO_push(b64, bio);

    size_t total_size = 0;
    // Read the file data in chunks, storing them in a linked list
    struct link* head = malloc(sizeof(struct link));
    struct link *cur = head;
    while (!BIO_eof(b64)) {
        cur->data = malloc(sizeof(char) * CHUNK_SIZE);
        cur->data_len = BIO_read(b64, cur->data, CHUNK_SIZE);
        total_size += cur->data_len;
        cur->next = malloc(sizeof(struct link));
        cur->next->data = NULL;
        cur = cur->next;
    }

    // Now that the total size is finally know, allocate a buffer
    char *file_data = malloc(sizeof(char) * total_size);
    size_t file_data_offset = 0;
    // Move through the linked list copying data into the final buffer
    // free the links and their buffers as we go
    for (cur = head; cur->data != NULL;) {
        memcpy(&file_data[file_data_offset], cur->data, cur->data_len);        
        file_data_offset += cur->data_len;
        free(cur->data);
        struct link* prev = cur;
        cur = cur->next;
        free(prev);
    }
    free(cur);
    BIO_free_all(b64);

    *data_size = total_size;
    return file_data;
}

int main() 
{
    size_t total_size;
    char *file_data = load_base64_file(stdin, &total_size);

    size_t best_key_size = 0; 
    float smallest_dist = 1.0/best_key_size; // set to infinity
    for (size_t key_size = 1; key_size < MAX_KEY_SIZE; key_size++) {
        float dist_avg = 0;
        int count = 0;
        /* Slide down the file one 'key_size' chunk at a time and compare the hamming distance
            to the neighboring chunk and build up an average */
        for (int offset = 0; offset + key_size - 1 < total_size; offset += key_size, count++) {
            dist_avg += hamming_distance(&file_data[offset], &file_data[offset+key_size], key_size);
        }
        dist_avg /= count * key_size;
        /* When encrypting text, the correct key will yield a smaller hamming distance than
            then encrypting uniformly random data. Look for the key_len with the smallest
            average hamming distance */
        if (dist_avg < smallest_dist) {
            smallest_dist = dist_avg;
            best_key_size = key_size;
            printf("key len: %zd, hd: %f\n", key_size, dist_avg);
        }
    }

    /* crack the file one byte at a time by creating strings that were all encrypted
        with the same byte. Basically re-dim the file using key_len as the width
        and treating the columns as individual cyphertexts encrypted by a single byte.*/
    printf("Key: ");
    char *key = malloc(sizeof(char) * best_key_size);
    for (int i=0; i < best_key_size; i++) {
        char *buff = malloc(sizeof(char) * total_size / best_key_size);
        for (int j=i, k=0; j < total_size; j+= best_key_size)
            buff[k++] = file_data[j];
        struct byte_xor_chunk key_part = byte_xor_crack(buff, total_size / best_key_size);
        key[i] = key_part.key;
        putc(key[i], stdout);
    }

    printf("\nPlaintext:\n");
    for (size_t i=0; i < total_size; i++) {
        putc(file_data[i] ^ key[i%best_key_size], stdout);
    }

    return 0;
}
