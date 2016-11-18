#ifndef __TCPCLIENT_H
#define __TCPCLIENT_H

#ifdef EXT_TCPCLIENT
#define EXT_TCPCLIENT
#else
#define EXT_TCPCLIENT extern
#endif

/* 头文件包含 */
#include <stdint.h>

typedef struct{
  int    clientNum;
  char*  serverAddr;
  int    serverPort;
  char*  msg;
}tcpclient_arg_t;
EXT_TCPCLIENT tcpclient_arg_t tcpclient_arg;

/* 公有函数声明 */

#endif /* #ifndef __TCPCLIENT_H */
