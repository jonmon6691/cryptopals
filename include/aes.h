#ifndef AES_H
#define AES_H
#include <inttypes.h>

union block {
    uint8_t bytes[16];
    uint32_t words[4]; // column words
    uint8_t grid[4][4]; // [cols][rows]
};

struct aes_state {
    /* Maintain 2 state blocks so that operations that can't be done 
        in place can be done in the other buffer then pointer-swapped*/
    union block state[2];
    uint8_t si; // State index. Keeps track of the current state buffer offset
};

void aes_128_block_encrypt(char *input, char *output, char *key);

uint8_t s_box(uint8_t s);

uint8_t aes_mult(uint8_t a, uint8_t b);

uint32_t aes_round_constant(unsigned int round);

uint32_t aes_key_schedule(unsigned int round);

void aes_sub_bytes(struct aes_state *state);

void aes_shift_rows(struct aes_state * state);

void aes_print_block(struct aes_state * state);

#endif