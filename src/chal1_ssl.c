#include <stdio.h>
#include "crypto_tools.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/applink.c>

char *input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
char *input_bytes;

int main() 
{
    size_t input_size = str2bytes(input, &input_bytes);
    BIO *bio, *b64;
    char message[] = "Hello World \n";

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_fp(stdout, BIO_NOCLOSE);
    
    BIO_push(b64, bio);
    BIO_write(b64, input_bytes, input_size);
    BIO_flush(b64);
    
    BIO_free_all(b64);
    free(input_bytes);
    return 0;
}
