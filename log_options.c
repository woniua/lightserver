#define EXT_LOG_OPTIONS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "log_options.h"
#include "list.h"
#include "config.h"

/******************************************************************************/
// example:
// 2016-11-23 11:32:45 126.12.3.4:3456(name) connect:56th on .
// 2016-11-23 11:32:45 126.12.3.4:3456(name) disconnect:remote closed it.
// 2016-11-23 11:32:45 126.12.3.4:3456(name) recv-string(14):hello world!
// 2016-11-23 11:32:45 126.12.3.4:3456(name) send-string(14):hello world!
// 2016-11-23 11:32:45 126.12.3.4:3456(name) recv-hex(3):11 22 33
// 2016-11-23 11:32:45 126.12.3.4:3456(name) send-hex(3):11 22 33
// 2016-11-23 11:32:45 126.12.3.4:3456(name) system-error:mysql insert failed.
/******************************************************************************/
static void current_date_format(char* outbuf)
{
  time_t     crt_time;
  struct tm* crt_tm_time;

  time(&crt_time);
  crt_tm_time = localtime(&crt_time);
  sprintf(&outbuf[strlen(outbuf)], "%04d-", crt_tm_time->tm_year+1900);
  sprintf(&outbuf[strlen(outbuf)], "%02d-", crt_tm_time->tm_mon+1);
  sprintf(&outbuf[strlen(outbuf)], "%02d ", crt_tm_time->tm_mday);
  sprintf(&outbuf[strlen(outbuf)], "%02d:", crt_tm_time->tm_hour);
  sprintf(&outbuf[strlen(outbuf)], "%02d:", crt_tm_time->tm_min);
  sprintf(&outbuf[strlen(outbuf)], "%02d",  crt_tm_time->tm_sec);
}

static void print_in_hex(uint8_t* bufin, uint32_t bufin_len)
{
  uint32_t i;
  int8_t   print_buf[3];

  for(i = 0; i < bufin_len; i++){
    sprintf(print_buf, "%02x", *(bufin + i));
    printf("%s ", print_buf);
  }
}

static uint32_t sprint_in_hex(uint8_t* bufin, uint32_t bufin_len, uint8_t* bufout)
{
  uint32_t i;
  int8_t   print_buf[3];

  for(i = 0; i < bufin_len; i++){
    sprintf(&bufout[strlen(bufout)], "%02x ", *(bufin + i));
  }
  return strlen(bufout);
}

//成功返回0，失败返回-1
static int32_t get_nodedata(int32_t port_type, int32_t fd, nodedata_t* nodedata)
{
  int32_t index;

  if(port_type == CFG_HTTP_PORT){
    index = list_check_nodedata(&http_head, fd, nodedata);
    if(index == -1){
      fprintf(stderr, "check http_list failed on function '%s'\n", __FUNCTION__);
      return -1;
    }
  }else if(port_type == CFG_TCP_DEV_PORT){
    index = list_check_nodedata(&tcp_dev_head, fd, nodedata);
    if(index == -1){
      fprintf(stderr, "check tcp_dev_list failed on function '%s'\n", __FUNCTION__);
      return -1;
    }
  }else if(port_type == CFG_TCP_MONITOR_PORT){
    index = list_check_nodedata(&tcp_monitor_head, fd, nodedata);
    if(index == -1){
      fprintf(stderr, "check tcp_monitor_list failed on function '%s'\n", __FUNCTION__);
      return -1;
    }
  }else{
    fprintf(stderr, "unknown port_type: %d.\n", port_type);
    return -1;
  }
  return 0;
}

static void get_record_prompt(log_type_t log_type, uint32_t msg_len, char* bufout)
{
  if(log_type == LOG_TYPE_CONNECT){
    sprintf(bufout, "%s", "connect:");
  }
  else if(log_type == LOG_TYPE_DISCONNECT){
    sprintf(bufout, "%s", "disconnect:");
  }
  else if(log_type == LOG_TYPE_RECV_STRING){
    sprintf(bufout, "recv-string(%d):\n", msg_len);
  }
  else if(log_type == LOG_TYPE_SEND_STRING){
    sprintf(bufout, "send-string(%d):\n", msg_len);
  }
  else if(log_type == LOG_TYPE_RECV_HEX){
    sprintf(bufout, "recv-hex(%d):\n", msg_len);
  }
  else if(log_type == LOG_TYPE_SEND_HEX){
    sprintf(bufout, "send-hex(%d):\n", msg_len);
  }
  else if(log_type == LOG_TYPE_ERROR){
    sprintf(bufout, "%s", "system-error:");
  }
  else{
    fprintf(stderr, "unknown log_type: %d.\n", log_type);
  }
}

static void get_record_name(int32_t fd, uint32_t port_type, nodedata_t nodedata, char* bufout)
{
  if(port_type == CFG_TCP_DEV_PORT){
    if(CFG_EN_TCP_DEV_ID_AUTH == 0){
      sprintf(bufout, "%s:%d(%s)", inet_ntoa(nodedata.saddr.sin_addr), \
                       nodedata.saddr.sin_port, \
                       (nodedata.name == NULL) ? "NULL" : nodedata.name);
    }
    else{
      sprintf(bufout, "%s", nodedata.name);
    }
  }
  else if(port_type == CFG_TCP_MONITOR_PORT){
    if(CFG_EN_TCP_MONITOR_ID_AUTH == 0){
      sprintf(bufout, "%s:%d(%s)", inet_ntoa(nodedata.saddr.sin_addr), \
                       nodedata.saddr.sin_port, \
                       (nodedata.name == NULL) ? "NULL" : nodedata.name);
    }
    else{
      sprintf(bufout, "%s", nodedata.name);
    }
  }
  else{
    fprintf(stderr, "unknown port_type: %d.\n", port_type);
  }
}

static void get_record_msg(log_type_t log_type, uint32_t port_type, const int8_t* msg, \
                           uint32_t msg_len, nodedata_t nodedata, char* bufout)
{
  if(port_type == CFG_TCP_DEV_PORT){
    if((log_type == LOG_TYPE_CONNECT)||(log_type == LOG_TYPE_DISCONNECT)){
      if(CFG_EN_TCP_DEV_ID_AUTH == 1){
        sprintf(&bufout[strlen(bufout)], "%s:%d ", \
                inet_ntoa(nodedata.saddr.sin_addr), nodedata.saddr.sin_port);
      }
      sprintf(&bufout[strlen(bufout)], "%s", msg);
    }
    else if((log_type == LOG_TYPE_RECV_STRING)||(log_type == LOG_TYPE_SEND_STRING)){
      sprintf(&bufout[strlen(bufout)], "%s", msg);
    }
    else if((log_type == LOG_TYPE_RECV_HEX)||(log_type == LOG_TYPE_SEND_HEX)){
      sprint_in_hex((int8_t*)msg, msg_len, bufout);
    }
    else if(log_type == LOG_TYPE_ERROR){
      sprintf(&bufout[strlen(bufout)], "%s", msg);
    }
    else{
      fprintf(stderr, "unknown log_type: %d.\n", log_type);
    }
  }
  else if(port_type == CFG_TCP_MONITOR_PORT){
    if((log_type == LOG_TYPE_CONNECT)||(log_type == LOG_TYPE_DISCONNECT)){
      if(CFG_EN_TCP_MONITOR_ID_AUTH == 1){
        sprintf(&bufout[strlen(bufout)], "%s:%d ", \
                inet_ntoa(nodedata.saddr.sin_addr), nodedata.saddr.sin_port);
      }
      sprintf(&bufout[strlen(bufout)], "%s", msg);
    }
    else if((log_type == LOG_TYPE_RECV_STRING)||(log_type == LOG_TYPE_SEND_STRING)){
      sprintf(&bufout[strlen(bufout)], "%s", msg);
    }
    else if((log_type == LOG_TYPE_RECV_HEX)||(log_type == LOG_TYPE_SEND_HEX)){
      sprint_in_hex((int8_t*)msg, msg_len, bufout);
    }
    else if(log_type == LOG_TYPE_ERROR){
      sprintf(&bufout[strlen(bufout)], "%s", msg);
    }
    else{
      fprintf(stderr, "unknown log_type: %d.\n", log_type);
    }
  }
  else{
    fprintf(stderr, "unknown port_type: %d.\n", port_type);
  }
}

static void log_print(uint32_t port_type, log_record_t record)
{
  if((port_type == CFG_TCP_DEV_PORT)&&(CFG_EN_TCP_DEV_LOG_PRINT == 1)){
    printf("%s %s %s %s\n", record.datetime, record.name, record.prompt, record.msg);
  }
  if((port_type == CFG_TCP_MONITOR_PORT)&&(CFG_EN_TCP_MONITOR_LOG_PRINT == 1)){
    printf("%s %s %s %s\n", record.datetime, record.name, record.prompt, record.msg);
  }
}

static void log_record(uint32_t port_type, log_record_t record)
{
  FILE*       fp;
  int8_t      record_buf[sizeof(log_record_t)+128];

  if((port_type == CFG_TCP_DEV_PORT)&&(CFG_EN_TCP_DEV_LOG_RECORD == 1)){
    sprintf(record.record_file, "log/dev/%s.txt", record.name);
    fp = fopen(record.record_file, "a+");
    if(fp == NULL){
      fprintf(stderr, "open log_file %s failed!", record.record_file);
      perror(" ");
      return;
    }
    sprintf(record_buf, "%s %s %s %s\n", record.datetime, record.name, \
                                         record.prompt  , record.msg);
    fputs(record_buf, fp);
    fclose(fp);
  }
  if((port_type == CFG_TCP_MONITOR_PORT)&&(CFG_EN_TCP_MONITOR_LOG_RECORD == 1)){
    sprintf(record.record_file, "log/monitor/%s.txt", record.name);
    fp = fopen(record.record_file, "a+");
    if(fp == NULL){
      fprintf(stderr, "open log_file %s failed!", record.record_file);
      perror(" ");
      return;
    }
    sprintf(record_buf, "%s %s %s %s\n", record.datetime, record.name, \
                                         record.prompt  , record.msg);
    fputs(record_buf, fp);
    fclose(fp);
  }
}

static void log_connect(log_type_t log_type, uint32_t port_type, int32_t fd, const int8_t* msg, uint32_t msg_len)
{
  log_record_t  record   = {0};
  nodedata_t    nodedata = {0};

  if(-1 == get_nodedata(port_type, fd, &nodedata))return;
  current_date_format(record.datetime);
  get_record_prompt(log_type, msg_len, record.prompt);
  get_record_name(fd, port_type, nodedata, record.name);
  get_record_msg(log_type, port_type, msg, msg_len, nodedata, record.msg);
  //日志打印
  log_print(port_type, record);
  //日志记录
  log_record(port_type, record);
}

static void log_disconnect(log_type_t log_type, uint32_t port_type, int32_t fd, const int8_t* msg, uint32_t msg_len)
{
  log_record_t  record   = {0};
  nodedata_t    nodedata = {0};

  if(-1 == get_nodedata(port_type, fd, &nodedata))return;
  current_date_format(record.datetime);
  get_record_prompt(log_type, msg_len, record.prompt);
  get_record_name(fd, port_type, nodedata, record.name);
  get_record_msg(log_type, port_type, msg, msg_len, nodedata, record.msg);
  //日志打印
  log_print(port_type, record);
  //日志记录
  log_record(port_type, record);
}

static void log_recv_string(log_type_t log_type, uint32_t port_type, int32_t fd, const int8_t* msg, uint32_t msg_len)
{
  log_record_t  record   = {0};
  nodedata_t    nodedata = {0};

  if(-1 == get_nodedata(port_type, fd, &nodedata))return;
  current_date_format(record.datetime);
  get_record_prompt(log_type, msg_len, record.prompt);
  get_record_name(fd, port_type, nodedata, record.name);
  get_record_msg(log_type, port_type, msg, msg_len, nodedata, record.msg);
  //日志打印
  log_print(port_type, record);
  //日志记录
  log_record(port_type, record);
}

static void log_send_string(log_type_t log_type, uint32_t port_type, int32_t fd, const int8_t* msg, uint32_t msg_len)
{
  log_record_t  record   = {0};
  nodedata_t    nodedata = {0};

  if(-1 == get_nodedata(port_type, fd, &nodedata))return;
  current_date_format(record.datetime);
  get_record_prompt(log_type, msg_len, record.prompt);
  get_record_name(fd, port_type, nodedata, record.name);
  get_record_msg(log_type, port_type, msg, msg_len, nodedata, record.msg);
  //日志打印
  log_print(port_type, record);
  //日志记录
  log_record(port_type, record);
}

static void log_recv_hex(log_type_t log_type, uint32_t port_type, int32_t fd, const int8_t* msg, uint32_t msg_len)
{
  log_record_t    record   = {0};
  nodedata_t  nodedata = {0};

  if(-1 == get_nodedata(port_type, fd, &nodedata))return;
  current_date_format(record.datetime);
  get_record_prompt(log_type, msg_len, record.prompt);
  get_record_name(fd, port_type, nodedata, record.name);
  get_record_msg(log_type, port_type, msg, msg_len, nodedata, record.msg);
  //日志打印
  log_print(port_type, record);
  //日志记录
  log_record(port_type, record);
}

static void log_send_hex(log_type_t log_type, uint32_t port_type, int32_t fd, const int8_t* msg, uint32_t msg_len)
{
  log_record_t  record   = {0};
  nodedata_t    nodedata = {0};

  if(-1 == get_nodedata(port_type, fd, &nodedata))return;
  current_date_format(record.datetime);
  get_record_prompt(log_type, msg_len, record.prompt);
  get_record_name(fd, port_type, nodedata, record.name);
  get_record_msg(log_type, port_type, msg, msg_len, nodedata, record.msg);
  //日志打印
  log_print(port_type, record);
  //日志记录
  log_record(port_type, record);
}

static void log_error(log_type_t log_type, uint32_t port_type, int32_t fd, const int8_t* msg, uint32_t msg_len)
{
  log_record_t   record   = {0};
  nodedata_t     nodedata = {0};

  if(-1 == get_nodedata(port_type, fd, &nodedata))return;
  current_date_format(record.datetime);
  get_record_prompt(log_type, msg_len, record.prompt);
  get_record_name(fd, port_type, nodedata, record.name);
  get_record_msg(log_type, port_type, msg, msg_len, nodedata, record.msg);
  //日志打印
  log_print(port_type, record);
  //日志记录
  log_record(port_type, record);
}

void log_handle(log_type_t log_type, uint32_t port_type, int32_t fd, const int8_t* msg, uint32_t msg_len)
{
  if(log_type == LOG_TYPE_CONNECT){
    //客户端连接日志记录
    if((port_type == CFG_TCP_DEV_PORT)&& (CFG_EN_TCP_DEV_LOG_TYPE_CONNECT == 1)){
      log_connect(log_type, port_type, fd, msg, msg_len);
    }
    if((port_type == CFG_TCP_MONITOR_PORT)&&(CFG_EN_TCP_MONITOR_LOG_TYPE_CONNECT == 1)){
      log_connect(log_type, port_type, fd, msg, msg_len);
    }
  }
  else if(log_type == LOG_TYPE_DISCONNECT){
    //客户端断开连接日志记录
    if((port_type == CFG_TCP_DEV_PORT)&& (CFG_EN_TCP_DEV_LOG_TYPE_DISCONNECT == 1)){
        log_disconnect(log_type, port_type, fd, msg, msg_len);
    }
    if((port_type == CFG_TCP_MONITOR_PORT)&&(CFG_EN_TCP_MONITOR_LOG_TYPE_DISCONNECT == 1)){
        log_disconnect(log_type, port_type, fd, msg, msg_len);
    }
  }
  else if(log_type == LOG_TYPE_RECV_STRING){
    //客户端接收数据字符串显示/存储日志记录
    if((port_type == CFG_TCP_DEV_PORT)&& (CFG_EN_TCP_DEV_LOG_TYPE_RECV_STRING == 1)){
        log_recv_string(log_type, port_type, fd, msg, msg_len);
    }
    if((port_type == CFG_TCP_MONITOR_PORT)&&(CFG_EN_TCP_MONITOR_LOG_TYPE_RECV_STRING == 1)){
        log_recv_string(log_type, port_type, fd, msg, msg_len);
    }
  }
  else if(log_type == LOG_TYPE_SEND_STRING){
    //客户端发送数据字符串显示/存储日志记录
    if((port_type == CFG_TCP_DEV_PORT)&& (CFG_EN_TCP_DEV_LOG_TYPE_SEND_STRING == 1)){
        log_send_string(log_type, port_type, fd, msg, msg_len);
    }
    if((port_type == CFG_TCP_MONITOR_PORT)&&(CFG_EN_TCP_MONITOR_LOG_TYPE_SEND_STRING == 1)){
        log_send_string(log_type, port_type, fd, msg, msg_len);
    }
  }
  else if(log_type == LOG_TYPE_RECV_HEX){
    //客户端接收数据十六进制显示/存储日志记录
    if((port_type == CFG_TCP_DEV_PORT)&& (CFG_EN_TCP_DEV_LOG_TYPE_RECV_HEX == 1)){
        log_recv_hex(log_type, port_type, fd, msg, msg_len);
    }
    if((port_type == CFG_TCP_MONITOR_PORT)&&(CFG_EN_TCP_MONITOR_LOG_TYPE_RECV_HEX == 1)){
        log_recv_hex(log_type, port_type, fd, msg, msg_len);
    }
  }
  else if(log_type == LOG_TYPE_SEND_HEX){
    //客户端发送数据十六进制显示/存储日志记录
    if((port_type == CFG_TCP_DEV_PORT)&& (CFG_EN_TCP_DEV_LOG_TYPE_SEND_HEX == 1)){
        log_send_hex(log_type, port_type, fd, msg, msg_len);
    }
    if((port_type == CFG_TCP_MONITOR_PORT)&&(CFG_EN_TCP_MONITOR_LOG_TYPE_SEND_HEX == 1)){
        log_send_hex(log_type, port_type, fd, msg, msg_len);
    }
  }
  else if(log_type == LOG_TYPE_ERROR){
    //服务器运行错误日志记录
    if((port_type == CFG_TCP_DEV_PORT)&& (CFG_EN_TCP_DEV_LOG_TYPE_ERROR == 1)){
        log_error(log_type, port_type, fd, msg, msg_len);
    }
    if((port_type == CFG_TCP_MONITOR_PORT)&&(CFG_EN_TCP_MONITOR_LOG_TYPE_ERROR == 1)){
        log_error(log_type, port_type, fd, msg, msg_len);
    }
  }
  else{
    fprintf(stderr, "unknown log_type!\n");
  }
}
