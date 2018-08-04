/* AES in ECB mode
    
    The Base64-encoded content in this file has been encrypted via AES-128 in ECB mode under the key
    "YELLOW SUBMARINE".
    (case-sensitive, without the quotes; exactly 16 characters; I like "YELLOW SUBMARINE" because it's exactly 16 bytes long, and now you do too).

    Decrypt it. You know the key, after all.

    Easiest way: use OpenSSL::Cipher and give it AES-128-ECB as the cipher.

    Do this with code.
    You can obviously decrypt this using the OpenSSL command-line tool, but we're having you get ECB working in code for a reason. You'll need it a lot later on, and not just for attacking ECB.

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
