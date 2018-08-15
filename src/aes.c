/* An amateur implementation of AES-128 
    (c) 2018 Jonathan Wallace */
#include <stdlib.h>
#include <inttypes.h>
#include "aes.h"



void aes_128_block_encrypt(char *input, char *output, char *key)
{
    for (int i = 0; i < 256; i++) {
        printf("%d, %02hhx\n", i, (2 << (i-1)) % 0x11b);
    }    
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

uint32_t aes_round_constant(unsigned int round)
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
    return rcon << 24;
}

uint32_t aes_key_schedule(unsigned int round)
{
    return 0;
}

union block *dref(struct aes_state *s)
{
    // Convinience function, maybe should be a macro?
    return &s->state[s->si];
}

void aes_sub_bytes(struct aes_state * s)
{
    for (int i = 0; i < 16; i++)
        dref(s)->bytes[i] = s_box(dref(s)->bytes[i]);
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
}

void aes_mix_cols(struct aes_state * s)
{
    // swap buffers since this won't be done in-place
    uint8_t *start = dref(s)->bytes;
    s->si = !s->si;
    uint8_t *end = dref(s)->bytes;
    /* Multiply each column by the static polynomial in GF(2^8) */
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
}

void aes_print_block(struct aes_state * s)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%hhx ", dref(s)->grid[j][i]);
        }
        printf("\n");
    }
}

int main()
{
    struct aes_state s;
    s.si = 0;
    for (int i = 0; i < 16; i++) dref(&s)->bytes[i] = 0xd4;
    dref(&s)->words[0] = 0x4c31262d;
    dref(&s)->words[1] = 0x455313db;
    dref(&s)->words[2] = 0x5c220af2;
    aes_print_block(&s);
    printf("\n");
    aes_mix_cols(&s);
    aes_print_block(&s);
    printf("\n");
    aes_shift_rows(&s);
    aes_print_block(&s);
    return 0;
}