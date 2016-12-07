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
EXT_AES_OPTIONS  int  encrypt(char *input_string  , char **encrypt_string);
EXT_AES_OPTIONS  void decrypt(char *encrypt_string, char **decrypt_string, int len);
#endif /* #ifndef __AES_OPTIONS_H */
