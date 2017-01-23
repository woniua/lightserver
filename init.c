#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "listen_event.h"
#include "http_event.h"
#include "tcp_dev_event.h"
#include "tcp_monitor_event.h"
#include "mysql_options.h"
#include "aes_options.h"
#include "log_options.h"
#include "list.h"
#include "config.h"

int main(int argc, char const *argv[])
{
  MYSQL* mysql_fd = mysql_connect_init();
  mysql_demo_create_table(mysql_fd);
  //打印lightserver版本信息
  printf("Lightserver: %d.%d.%d\n",MAJOR_VERSION,MINOR_VERSION,REVISION_VERSION);

  //打印libevent版本信息
  printf("Used libevent version: %s\n", event_get_version());

  //建立日志路径
  system("mkdir -p log/dev mkdir -p log/monitor");

  //建立存储连接信息的链表
  list_create(&http_head, 0);
  list_create(&tcp_dev_head,  0);
  list_create(&tcp_monitor_head,  0);

  //创建http-event-dispath线程
  http_event_thread_create(1);

  //创建tcp-dev-event-dispath线程
  tcp_dev_event_thread_create(1);

  //创建tcp-monitor-event-dispath线程
  tcp_monitor_event_thread_create(1);

  //监听事件处理
  listen_event_handle();

  printf("done.\n");
  return 0;
}
