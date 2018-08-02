#include <inttypes.h>
#include <assert.h>

char *base64map = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define pd(s, d) printf("%s:%d ", s, d)

uint8_t is_wordchar(char a)
{
    return (a >= 'a' && a <= 'z' || a >= 'A' && a <= 'Z');
}

float plaintext_score(uint8_t *potential_plaintext, size_t input_length)
{
    float score = 0;
    for (int i=0; i< input_length; i++) {
        score += is_wordchar(potential_plaintext[i])?1:0;
    }
    return score / input_length;
}

uint8_t hexdig2int(char hexdig) {
    if (hexdig >= '0' && hexdig <= '9') {
        return hexdig - '0';
    } else if (hexdig >= 'a' && hexdig <= 'f') {
        return hexdig - 'a' + 10;
    } else if (hexdig >= 'A' && hexdig <= 'F') {
        return hexdig - 'A' + 10;
    } else return 0x10; // Error code
}


size_t str2bytes(char* input, char **output) {
    unsigned long inputsize = strnlen(input, 1024);
    size_t output_size = sizeof(uint8_t) * inputsize / 2;
    int offset = inputsize % 2; // If the input length is odd, assume a leading 0 nibble
    output_size += offset; // Adds a byte to the output for that nibble
    *output = (char*) malloc(output_size);
    if (offset) {
        (*output)[0] = hexdig2int(input[0]);
    }
    for(int i = 0; i < inputsize; i+=2) {
        (*output)[i/2+offset] = hexdig2int(input[i + offset]) * 16 + hexdig2int(input[i+1+offset]);
    }
    return output_size;
}


size_t bytes2base64(char *input, char **output, size_t input_length)
{
    // 0b12345678   90123456  78901234 Bytes (8 bits each)
    // 0b123456-78 9012-3456 78-901234 Base 64 digits (6 bits each)
    size_t output_size = sizeof(char) * input_length / 3 * 4;
    if (input_length % 3 != 0) // Padding needed, add bytes to the output size
        output_size += 4;
    *output = (char*) malloc (output_size + 1); // Add room for null terminator
    char* o = *output; // Convinient accessor variable
    for (int i=0; i < input_length; i+=3) {
        int j = i*4/3;
        o[j+0] = base64map[0x3f & (input[i] >> 2)];
        
        if (i+1 < input_length) {
            o[j+1] = base64map[(0x30 & input[i]<<4) | (0x0f & input[i+1]>>4)];
        } else {
            o[j+1] = base64map[0x30 & input[i]<<4];
            o[j+2] = '=';
            o[j+3] = '=';
            break;
        }
        if (i+2 < input_length) {
            o[j+2] = base64map[(0x3C & input[i+1] << 2) | (0x03 & input[i+2] >> 6)];
            o[j+3] = base64map[(0x3f & input[i+2])];
        } else {
            o[j+2] = base64map[0x3C & input[i+1] << 2];
            o[j+3] = '=';
        }
    }
    o[output_size] = '\0';
    return output_size;
}

