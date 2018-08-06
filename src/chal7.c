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
char *load_base64_aes_file(FILE *fp, size_t *data_size)
{
    // Set up OpenSSL base64 pipeline
    BIO *bio, *b64, *aes_ecb;

    aes_ecb = BIO_new(BIO_f_cipher());
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_fp(fp, BIO_NOCLOSE);
    
    // Set up the cipher filter for decryption of 128bit AES in ECB mode
    BIO_set_cipher(aes_ecb, EVP_aes_128_ecb(), "YELLOW SUBMARINE", 0, 0);
    
    // program <-> aes filter <-> base 64 filter <-> file pointer
    BIO_push(aes_ecb, b64);
    BIO_push(b64, bio);
    
    size_t total_size = 0;
    // Read the file data in chunks, storing them in a linked list
    struct link* head = malloc(sizeof(struct link));
    struct link *cur = head;
    while (!BIO_eof(aes_ecb)) {
        cur->data = malloc(sizeof(char) * CHUNK_SIZE);
        cur->data_len = BIO_read(aes_ecb, cur->data, CHUNK_SIZE);
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
    char *file_data = load_base64_aes_file(stdin, &total_size);

    printf("Length: %d\n", total_size);
    printf("Plaintext:\n");
    for (size_t i=0; i < total_size; i++) {
        putc(file_data[i], stdout);
    }

    return 0;
}
