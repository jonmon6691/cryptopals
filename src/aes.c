/* An amateur implementation of AES-128 
    (c) 2018 Jonathan Wallace */
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "aes.h"

void aes_128_block_decrypt(char *input, char *output, char *key)
{
    return;
}


void aes_128_block_encrypt(char *input, char *output, char *key)
{
    struct aes_state s;
    s.si = 0;
    s.debug = true;
    s.ks_index = 0;

    memccpy(dref(&s)->bytes, input, sizeof(char), 16);

    if (s.debug) {
        printf("Input\n");
        aes_print_block(&s);
    }

    aes_compute_key_schedule(&s, key);

    aes_add_round_key(&s);

    for (int round = 0; round < 9; round++) {
        printf("Round %d\n", round);
        aes_sub_bytes(&s);
        aes_shift_rows(&s);
        aes_mix_cols(&s);
        aes_add_round_key(&s);
    }

    aes_sub_bytes(&s);
    aes_shift_rows(&s);
    aes_add_round_key(&s);

    if (s.debug) {
        printf("Output\n");
        aes_print_block(&s);
    }

    memccpy(output, dref(&s)->bytes, sizeof(char), 16);
    return;
}

uint8_t s_box(uint8_t s)
{
    uint8_t mi_s;
    // Find the multiplicitive inverse of s
    if (s == 0) {
        // 1/0 is undefined so let s = 0
        mi_s = 0;
    } else {
        // Brute force search for M.I.
        for (mi_s = 1; aes_mult(s,  mi_s) != 1; mi_s++);
    }
    // Perform affine transformation of M.I.
    uint8_t r = 0;
    for (int i=0; i < 5; i++) {
        r ^= mi_s;
        mi_s = (mi_s << 1) | (mi_s >> 7);
    }
    return r ^ 0x63;
}

uint8_t aes_mult(uint8_t a, uint8_t b)
{
    uint8_t p = 0; // product
    while (a && b) {
        if (b & 0b1) {
            p ^= a; // Addition in GF(2^8)
        }
        b >>= 1;
        if (a & 0x80) {
            a = (a << 1) ^ 0x11b; // ...finite field modulo magic?
        } else {
            a = a << 1;
        }
    }
    return p;
}

uint8_t aes_round_constant(unsigned int round)
{
    if (round == 0) {
        /* 0x00 is problematic to calculate, but the round constant is 
            periodic so we can take the value from 360 degrees down 
            the sequence */
        round = 0x33;
    }
    uint8_t rcon = 1;
    for (round--; round; round--)
        // round constant is just powers of 2 on GF(2^8)
        rcon = aes_mult(rcon, 0x02);
    return rcon;
}


union block *dref(struct aes_state *s)
{
    // Convinience function, maybe should be a macro?
    return &s->state[s->si];
}

void aes_compute_key_schedule(struct aes_state *s, char *key)
{
    // First 16 bytes of key schedule are the actual key
    for (int i = 0; i < 16; i++)
        ((char *)s->key_sched)[i] = key[i];
    // Generate key material for the rounds and key-whitening
    for (int i = 4; i < 44; i++) {
        uint32_t temp;
        char * temp_b = &(char)temp; // Easier byte-indexing into temp
        temp = s->key_sched[i - 1];
        if (i % 4 == 0) {
            temp = temp >> 8 | temp << 24;
            for (int j = 0; j < 4; j++)
                temp_b[j] = s_box(temp_b[j]);
            temp_b[0] ^= aes_round_constant(i / 4);
        }
        s->key_sched[i] = temp ^ s->key_sched[i - 4];
    }
    if (s->debug) {
        printf("aes_compute_key_schedule:\n");
        for (int i = 0; i < 44; i++) {
            for( int j = 0; j < 4; j++) {
                printf("%02hhx", ((char *)s->key_sched)[i*4+j]);
            }
            printf("\n");
        }
    }
    return;
}

void aes_add_round_key(struct aes_state * s)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dref(s)->bytes[i*4+j] ^= ((char *)s->key_sched)[s->ks_index*4+j];
        }
        s->ks_index++;
    }
    if (s->debug) {
        printf("aes_add_round_key\n");
        aes_print_block(s);
    }
    return;
}

void aes_sub_bytes(struct aes_state * s)
{
    for (int i = 0; i < 16; i++)
        dref(s)->bytes[i] = s_box(dref(s)->bytes[i]);

    if (s->debug) {
        printf("aes_sub_bytes\n");
        aes_print_block(s);
    }
}

void aes_shift_rows(struct aes_state * s)
{
    // Swap buffers since this won't be done in-place
    uint8_t *start = dref(s)->bytes;
    s->si = !s->si;
    uint8_t *end = dref(s)->bytes;
    // Shift each row to the left by n bytes where n is it's own row index
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            // end[row * 4 + col]
            end[col*4 + row] = start[row + ((row + col) % 4) * 4];
        }
    }
    if (s->debug) {
        printf("aes_shift_rows\n");
        aes_print_block(s);
    }
}

void aes_mix_cols(struct aes_state * s)
{
    // swap buffers since this won't be done in-place
    uint8_t *start = dref(s)->bytes;
    s->si = !s->si;
    uint8_t *end = dref(s)->bytes;
    /* Multiply each column by this static polynomial in GF(2^8) */
    uint8_t mix_cols_poly[4] = {2, 3, 1, 1};
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            end[4*col+row] = 0;
            /* Matrix multiply, right-shifting the polynomial and computing the dot product with
                the column repeatedly adding it back into the result column */
            for (int row2 = 0; row2 < 4; row2++) {
                end[4*col+row] ^= aes_mult(mix_cols_poly[(row2 - row + 4) % 4], start[4*col+row2]);
            }
        }
    }
    if (s->debug) {
        printf("aes_mix_cols\n");
        aes_print_block(s);
    }
}

void aes_print_block(struct aes_state * s)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%02hhx ", dref(s)->grid[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}


int test()
{
#if 1
    char input[] =    {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
    char key[] =      {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    char expected[] = {0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32};
#else
    char input[] =     {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    char key[] =       {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    char expected[] =  {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};
#endif
    aes_128_block_encrypt(input, input, key);
    if(memcmp(input, expected, 16) == 0)
        printf("Pass.");
    else
        printf("Fail.");

    return 0;
}