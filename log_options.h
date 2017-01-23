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

//日志类型
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

//日志记录缓冲区结构体
typedef struct{
  int8_t  datetime[20];      //日期时间
  int8_t  name[64];          //产生日志的主体
  int8_t  prompt[64];        //日志类型的提示
  int8_t  msg[2048];         //日志的描述
  int8_t  record_file[256];  //记录日志的文件路径
}log_record_t;

/* 公有函数声明 */
//msg_len:取值为零时表示此参数无意义
EXT_LOG_OPTIONS  void log_handle(log_type_t log_type, uint32_t port_type, \
                                 int32_t fd, const int8_t* msg, uint32_t msg_len);

#endif /* #ifndef __LOG_OPTIONS_H */
