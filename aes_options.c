#define EXT_AES_OPTIONS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

#include "aes_options.h"
#include "config.h"

int encrypt(char *input_string, char **encrypt_string)
{
  AES_KEY   aes;
  uint8_t   key[AES_BLOCK_SIZE];
  uint8_t   init_vector[AES_BLOCK_SIZE];
  uint32_t  len = 0;        //加密长度，为AES_BLOCK_SIZE的整数倍
  uint32_t  i;

  //设置加密长度
  len = (strlen(input_string)/AES_BLOCK_SIZE + 1)*AES_BLOCK_SIZE;
  //生成AES 128位密钥
  for(i = 0; i < AES_BLOCK_SIZE; i++){
    key[i] = 32 + i;
  }
  //设置初始化向量
  for(i = 0; i < AES_BLOCK_SIZE; i++){
    init_vector[i] = 0;
  }
  if(AES_set_encrypt_key(key, 128, &aes) < 0){
    fprintf(stderr, "Unable to set encryption key in AES\n");
    exit(0);
  }

  // alloc encrypt_string
  *encrypt_string = (uint8_t*)calloc(len, sizeof(uint8_t));
  if(*encrypt_string == NULL){
    fprintf(stderr, "Unable to allocate memory for encrypt_string\n");
    exit(-1);
  }

  // encrypt (init_vector will change)
  AES_cbc_encrypt(input_string, *encrypt_string, len, &aes, init_vector, AES_ENCRYPT);
  return len;
}

void decrypt(char *encrypt_string, char **decrypt_string,int len)
{
  AES_KEY  aes;
  uint8_t  key[AES_BLOCK_SIZE];
  uint8_t  init_vector[AES_BLOCK_SIZE];
  int      i;

  //生成AES 128位密钥
  for(i = 0; i < AES_BLOCK_SIZE; i++){
    key[i] = 32 + i;
  }

  // alloc decrypt_string
  *decrypt_string = (uint8_t*)calloc(len, sizeof(uint8_t));
  if(*decrypt_string == NULL){
    fprintf(stderr, "Unable to allocate memory for decrypt_string\n");
    exit(-1);
  }

  // Set decryption key
  for(i=0; i<AES_BLOCK_SIZE; i++){
    init_vector[i] = 0;
  }
  if(AES_set_decrypt_key(key, 128, &aes) < 0){
    fprintf(stderr, "Unable to set decryption key in AES\n");
    exit(-1);
  }

  // decrypt
  AES_cbc_encrypt(encrypt_string, *decrypt_string, len, &aes, init_vector, AES_DECRYPT);
}
