#define EXT_AES_OPTIONS

#include <stdio.h>
#include <openssl/aes.h>
#include <stdlib.h>
#include <string.h>

#include "aes_options.h"
#include "config.h"

int encrypt(char *input_string, char **encrypt_string)
{
    AES_KEY   aes;
    uint8_t   key[AES_BLOCK_SIZE];      // AES_BLOCK_SIZE = 16
    uint8_t   iv[AES_BLOCK_SIZE];       // init vector
    uint32_t  len;        // encrypt length (in multiple of AES_BLOCK_SIZE)
    uint32_t  i;

    // set the encryption length
    len = 0;
    if ((strlen(input_string) + 1) % AES_BLOCK_SIZE == 0){
        len = strlen(input_string) + 1;
    }
    else{
        len = ((strlen(input_string) + 1) / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
    }

    // Generate AES 128-bit key
    for (i=0; i<16; ++i) {
        key[i] = 32 + i;
    }

    // Set encryption key
    for (i=0; i<AES_BLOCK_SIZE; ++i) {
        iv[i] = 0;
    }
    if (AES_set_encrypt_key(key, 128, &aes) < 0) {
        fprintf(stderr, "Unable to set encryption key in AES\n");
        exit(0);
    }

    // alloc encrypt_string
    *encrypt_string = (uint8_t*)calloc(len, sizeof(uint8_t));
    if (*encrypt_string == NULL) {
        fprintf(stderr, "Unable to allocate memory for encrypt_string\n");
        exit(-1);
    }

    // encrypt (iv will change)
    AES_cbc_encrypt(input_string, *encrypt_string, len, &aes, iv, AES_ENCRYPT);
    return len;
}

void decrypt(char *encrypt_string, char **decrypt_string,int len)
{
    uint8_t key[AES_BLOCK_SIZE];        // AES_BLOCK_SIZE = 16
    uint8_t iv[AES_BLOCK_SIZE];        // init vector
    AES_KEY aes;
    int i;
    // Generate AES 128-bit key

    for (i=0; i<16; ++i) {
        key[i] = 32 + i;
    }

    // alloc decrypt_string
    *decrypt_string = (uint8_t*)calloc(len, sizeof(uint8_t));
    if (*decrypt_string == NULL) {
        fprintf(stderr, "Unable to allocate memory for decrypt_string\n");
        exit(-1);
    }

    // Set decryption key
    for (i=0; i<AES_BLOCK_SIZE; ++i) {
        iv[i] = 0;
    }
    if (AES_set_decrypt_key(key, 128, &aes) < 0) {
        fprintf(stderr, "Unable to set decryption key in AES\n");
        exit(-1);
    }

    // decrypt
    AES_cbc_encrypt(encrypt_string, *decrypt_string, len, &aes, iv,
            AES_DECRYPT);
}
