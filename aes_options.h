#ifndef __AES_OPTIONS_H
#define __AES_OPTIONS_H

#ifdef  EXT_AES_OPTIONS
#define EXT_AES_OPTIONS
#else
#define EXT_AES_OPTIONS extern
#endif

/* 头文件包含 */
#include <stdint.h>

/* 公有函数声明 */
//返回值:加密后的数据长度，如加密失败返回-1
EXT_AES_OPTIONS  int32_t encrypt(char* input, char* output, uint32_t input_len);
//返回值:解密后的数据长度，如解密失败返回-1
EXT_AES_OPTIONS  int32_t decrypt(char* input, char* output, uint32_t input_len);

#endif /* #ifndef __AES_OPTIONS_H */
