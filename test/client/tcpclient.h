#ifndef __TCPCLIENT_H
#define __TCPCLIENT_H

#ifdef EXT_TCPCLIENT
#define EXT_TCPCLIENT
#else
#define EXT_TCPCLIENT extern
#endif

/* 头文件包含 */
#include <stdint.h>

#define SOCKET_SEND_TIMEOUT_S                    5
#define SOCKET_RECV_TIMEOUT_S                    1

uint32_t recvcount;//统计累计接收数据次数

typedef struct{
  int    clientNum;
  char*  serverAddr;
  int    serverPort;
  char*  msg;
}tcpclient_arg_t;
EXT_TCPCLIENT tcpclient_arg_t tcpclient_arg;

/* 公有函数声明 */

#endif /* #ifndef __TCPCLIENT_H */
