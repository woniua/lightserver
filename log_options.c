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

static void print_in_hex(uint8_t* buf, uint32_t len)
{
  uint32_t i;
  int8_t   print_buf[3];

  for(i = 0; i < len; i++){
    sprintf(print_buf, "%02x", *(buf + i));
    printf("%s ", print_buf);
  }
}

static void log_connect(int32_t fd, uint32_t port_type, const int8_t* msg, uint32_t msg_len)
{
  FILE*       fp;
  char        datetime[20]     = {0};
  char        name[128]        = {0};
  char        type[]           = "connect:";
  char        record_file[156] = {0};
  char        record_buf[1024] = {0};
  nodedata_t  nodedata;

  current_date_format(datetime);
  if(-1 == get_nodedata(port_type, fd, &nodedata))return;
  sprintf(name, "%s:%d(%s)", inet_ntoa(nodedata.saddr.sin_addr), \
                             nodedata.saddr.sin_port, \
                             (nodedata.name == NULL) ? "NULL" : nodedata.name);
#if(CFG_EN_LOG_PRINT == 1)
  printf("%s %s %s %s\n", datetime, name, type, msg);
#endif

#if(CFG_EN_LOG_RECORD == 1)
  sprintf(record_file, "log/%s.txt", name);
  fp = fopen(record_file, "a+");
  if(fp == NULL){
    fprintf(stderr, "open log_file %s failed!",record_file);
    perror(" ");
  }
  sprintf(record_buf, "%s %s %s %s\n", datetime, name, type, msg);
  fputs(record_buf, fp);
  fclose(fp);
#endif
}

static void log_disconnect(int32_t fd, uint32_t port_type, const int8_t* msg, uint32_t msg_len)
{
  FILE*       fp;
  char        datetime[20]     = {0};
  char        name[128]        = {0};
  char        type[]           = "disconnect:";
  char        record_file[156] = {0};
  char        record_buf[1024] = {0};
  nodedata_t  nodedata;

  current_date_format(datetime);
  if(-1 == get_nodedata(port_type, fd, &nodedata))return;
  sprintf(name, "%s:%d(%s)", inet_ntoa(nodedata.saddr.sin_addr), \
                             nodedata.saddr.sin_port, \
                             (nodedata.name == NULL) ? "NULL" : nodedata.name);
#if(CFG_EN_LOG_PRINT == 1)
  printf("%s %s %s %s\n", datetime, name, type, msg);
#endif

#if(CFG_EN_LOG_RECORD == 1)
  sprintf(record_file, "log/%s.txt", name);
  fp = fopen(record_file, "a+");
  if(fp == NULL){
    fprintf(stderr, "open log_file %s failed!",record_file);
    perror(" ");
  }
  sprintf(record_buf, "%s %s %s %s\n", datetime, name, type, msg);
  fputs(record_buf, fp);
  fclose(fp);
#endif
}

static void log_recv_string(int32_t fd, uint32_t port_type, const int8_t* msg, uint32_t msg_len)
{
  FILE*       fp;
  char        datetime[20]     = {0};
  char        name[128]        = {0};
  char        type[64]         = {0};
  char        record_file[156] = {0};
  char        record_buf[1024] = {0};
  nodedata_t  nodedata;

  current_date_format(datetime);
  if(-1 == get_nodedata(port_type, fd, &nodedata))return;
  sprintf(name, "%s:%d(%s)", inet_ntoa(nodedata.saddr.sin_addr), \
                             nodedata.saddr.sin_port, \
                             (nodedata.name == NULL) ? "NULL" : nodedata.name);
  sprintf(type, "recv-string(%d):\n", msg_len);
#if(CFG_EN_LOG_PRINT == 1)
  printf("%s %s %s %s\n", datetime, name, type, msg);
#endif

#if(CFG_EN_LOG_RECORD == 1)
  sprintf(record_file, "log/%s.txt", name);
  fp = fopen(record_file, "a+");
  if(fp == NULL){
    fprintf(stderr, "open log_file %s failed!",record_file);
    perror(" ");
  }
  sprintf(record_buf, "%s %s %s %s\n", datetime, name, type, msg);
  fputs(record_buf, fp);
  fclose(fp);
#endif
}

static void log_send_string(int32_t fd, uint32_t port_type, const int8_t* msg, uint32_t msg_len)
{
  FILE*       fp;
  char        datetime[20]     = {0};
  char        name[128]        = {0};
  char        type[64]         = {0};
  char        record_file[156] = {0};
  char        record_buf[1024] = {0};
  nodedata_t  nodedata;

  current_date_format(datetime);
  if(-1 == get_nodedata(port_type, fd, &nodedata))return;
  sprintf(name, "%s:%d(%s)", inet_ntoa(nodedata.saddr.sin_addr), \
                             nodedata.saddr.sin_port, \
                             (nodedata.name == NULL) ? "NULL" : nodedata.name);
  sprintf(type, "send-string(%d):\n", msg_len);
#if(CFG_EN_LOG_PRINT == 1)
  printf("%s %s %s %s\n", datetime, name, type, msg);
#endif

#if(CFG_EN_LOG_RECORD == 1)
  sprintf(record_file, "log/%s.txt", name);
  fp = fopen(record_file, "a+");
  if(fp == NULL){
    fprintf(stderr, "open log_file %s failed!",record_file);
    perror(" ");
  }
  sprintf(record_buf, "%s %s %s %s\n", datetime, name, type, msg);
  fputs(record_buf, fp);
  fclose(fp);
#endif
}

static void log_recv_hex(int32_t fd, uint32_t port_type, const int8_t* msg, uint32_t msg_len)
{

}

static void log_send_hex(int32_t fd, uint32_t port_type, const int8_t* msg, uint32_t msg_len)
{

}

static void log_error(int32_t fd, uint32_t port_type, const int8_t* msg, uint32_t msg_len)
{

}

void log_handle(log_type_t log_type, int32_t fd, uint32_t port_type, const int8_t* msg, uint32_t msg_len)
{
  if(log_type == LOG_TYPE_CONNECT)
  {
#if(CFG_EN_LOG_CONNECT == 1)
    log_connect(fd, port_type, msg, msg_len);
#endif
  }
  else if(log_type == LOG_TYPE_DISCONNECT)
  {
#if(CFG_EN_LOG_DISCONNECT == 1)
    log_disconnect(fd, port_type, msg, msg_len);
#endif
  }
  else if(log_type == LOG_TYPE_RECV_STRING)
  {
#if(CFG_EN_LOG_RECV_STRING == 1)
    log_recv_string(fd, port_type, msg, msg_len);
#endif
  }
  else if(log_type == LOG_TYPE_SEND_STRING)
  {
#if(CFG_EN_LOG_SEND_STRING == 1)
    log_send_string(fd, port_type, msg, msg_len);
#endif
  }
  else if(log_type == LOG_TYPE_RECV_HEX)
  {
#if(CFG_EN_LOG_RECV_HEX == 1)
    log_recv_hex(fd, port_type, msg, msg_len);
#endif
  }
  else if(log_type == LOG_TYPE_SEND_HEX)
  {
#if(CFG_EN_LOG_SEND_HEX == 1)
    log_send_hex(fd, port_type, msg, msg_len);
#endif
  }
  else if(log_type == LOG_TYPE_ERROR)
  {
#if(CFG_EN_LOG_ERROR == 1)
    log_error(fd, port_type, msg, msg_len);
#endif
  }
  else
  {
    fprintf(stderr, "unknown log_type!\n");
  }
}
