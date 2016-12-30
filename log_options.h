#ifndef __LOG_OPTIONS_H
#define __LOG_OPTIONS_H

#ifdef EXT_LOG_OPTIONS
#define EXT_LOG_OPTIONS
#else
#define EXT_LOG_OPTIONS extern
#endif

/* 头文件包含 */
#include <stdio.h>
#include <stdint.h>

typedef enum{
  LOG_TYPE_CONNECT = 0 ,
  LOG_TYPE_DISCONNECT  ,
  LOG_TYPE_RECV_STRING ,
  LOG_TYPE_SEND_STRING ,
  LOG_TYPE_RECV_HEX    ,
  LOG_TYPE_SEND_HEX    ,
  LOG_TYPE_ERROR       ,
  LOG_TYPE_COUNT
}log_type_t;

/* 公有函数声明 */
EXT_LOG_OPTIONS  void log_handle(log_type_t log_type, int32_t fd, \
                                 uint32_t port_type, const int8_t* msg, uint32_t msg_len);

#endif /* #ifndef __LOG_OPTIONS_H */
