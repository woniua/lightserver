#define EXT_AES_OPTIONS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

#include "aes_options.h"
#include "config.h"

int32_t encrypt(uint8_t* input, uint8_t* output, uint32_t input_len)
{
  AES_KEY   aes;
  uint8_t*  input_p;
  uint8_t   key[AES_BLOCK_SIZE];
  uint8_t   init_vector[AES_BLOCK_SIZE];
  uint32_t  output_len = 0;
  uint32_t  i;

  //设置加密长度
  output_len = ((input_len/AES_BLOCK_SIZE) + 1)*AES_BLOCK_SIZE;
  input_p = (uint8_t*)calloc(output_len, sizeof(uint8_t));
  if(!input_p){
    fprintf(stderr, "unable to calloc buffer for input data\n");
    return -1;
  }
  memcpy(input_p, input, input_len);
  //加密密钥(应和解密密钥一致)
  for(i = 0; i < AES_BLOCK_SIZE; i++){
    key[i] = 32 + i;
  }
  //设置初始化向量
  for(i = 0; i < AES_BLOCK_SIZE; i++){
    init_vector[i] = 0;
  }
  if(AES_set_encrypt_key(key, 128, &aes) < 0){
    fprintf(stderr, "unable to set encryption key in aes\n");
    free(input_p);
    return -1;
  }
  //加密
  AES_cbc_encrypt(input_p, output, output_len, &aes, init_vector, AES_ENCRYPT);
  free(input_p);
  return output_len;
}

int32_t decrypt(uint8_t* input, uint8_t* output, uint32_t input_len)
{
  AES_KEY  aes;
  uint8_t  key[AES_BLOCK_SIZE];
  uint8_t  init_vector[AES_BLOCK_SIZE];
  int      i;

  //解密密钥(应和加密密钥一致)
  for(i = 0; i < AES_BLOCK_SIZE; i++){
    key[i] = 32 + i;
  }
  //设置初始化向量
  for(i=0; i<AES_BLOCK_SIZE; i++){
    init_vector[i] = 0;
  }
  if(AES_set_decrypt_key(key, 128, &aes) < 0){
    fprintf(stderr, "unable to set decryption key in aes\n");
    return -1;
  }
  //解密
  AES_cbc_encrypt(input, output, input_len, &aes, init_vector, AES_DECRYPT);

  return input_len;
}
