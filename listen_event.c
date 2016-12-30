#define EXT_LISTEN_EVENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>

#include "listen_event.h"
#include "tcp_dev_event.h"
#include "tcp_monitor_event.h"
#include "mysql_options.h"
#include "log_options.h"
#include "list.h"
#include "config.h"

void listener_http_cb(struct evconnlistener* listener, evutil_socket_t new_socket_fd,\
                      struct sockaddr* saddr, int socklen, void* arg)
{
  nodedata_t nodedata;

  //设置socket为非阻塞
  evutil_make_socket_nonblocking(new_socket_fd);
  //saddr赋值
  memcpy((void*)&nodedata, (const void*)saddr, sizeof(struct sockaddr));
  nodedata.connect_fd   = new_socket_fd;
  //HTTP连接信息插入链表
  list_insert_nodedata(&http_head, 1, nodedata);
  //HTTP客户端连接日志信息记录
  log_handle(LOG_TYPE_CONNECT, new_socket_fd, CFG_HTTP_PORT, "connect success.", 0);
}

void listener_tcp_dev_cb(struct evconnlistener* listener, \
                         evutil_socket_t new_socket_fd, \
                         struct sockaddr* saddr, int socklen, void* arg)
{
  nodedata_t          nodedata;
  struct timeval      timeout_read  = {CFG_TCP_DEV_TIMEOUT_READ_S , \
                                       CFG_TCP_DEV_TIMEOUT_READ_US};
  struct timeval      timeout_write = {CFG_TCP_DEV_TIMEOUT_WRITE_S, \
                                       CFG_TCP_DEV_TIMEOUT_WRITE_US};
  //设置socket为非阻塞
  evutil_make_socket_nonblocking(new_socket_fd);
  //saddr赋值
  memcpy((void*)&(nodedata.saddr), (const void*)saddr, sizeof(struct sockaddr));
  nodedata.connect_fd   = new_socket_fd;
#if(CFG_EN_TCP_DEV_MYSQL == 1)
  nodedata.mysql_fd     = mysql_connect_init();
#endif
  nodedata.socket_bufev = bufferevent_socket_new(tcp_dev_event_arg.tcp_eventbase, \
                                                 new_socket_fd, \
                                                 BEV_OPT_CLOSE_ON_FREE);
  if(!nodedata.socket_bufev){
    fprintf(stderr, "bufferevent create failed!\n");
  }

  bufferevent_setcb(nodedata.socket_bufev, \
                    bev_tcp_dev_read_cb, \
                    NULL, \
                    bev_tcp_dev_event_cb, \
                    NULL);
  bufferevent_set_timeouts(nodedata.socket_bufev, \
                           &timeout_read, \
                           &timeout_write);
  bufferevent_setwatermark(nodedata.socket_bufev, \
                           EV_READ , \
                           CFG_TCP_DEV_READ_LOWMARK, \
                           CFG_TCP_DEV_READ_HIGHMARK);
  bufferevent_enable(nodedata.socket_bufev, EV_READ|EV_WRITE);
  //TCP_DEV连接信息插入链表
  list_insert_nodedata(&tcp_dev_head, 1, nodedata);
  //客户端连接日志信息记录
  log_handle(LOG_TYPE_CONNECT, new_socket_fd, CFG_TCP_DEV_PORT, "connect success.", 0);
}

void listener_tcp_monitor_cb(struct evconnlistener* listener, \
                             evutil_socket_t new_socket_fd,\
                             struct sockaddr* saddr, int socklen, void* arg)
{
  nodedata_t          nodedata      = {0};
  struct timeval      timeout_read  = {CFG_TCP_MONITOR_TIMEOUT_READ_S , \
                                       CFG_TCP_MONITOR_TIMEOUT_READ_US};
  struct timeval      timeout_write = {CFG_TCP_MONITOR_TIMEOUT_WRITE_S, \
                                       CFG_TCP_MONITOR_TIMEOUT_WRITE_US};

  //设置socket为非阻塞
  evutil_make_socket_nonblocking(new_socket_fd);
  //saddr赋值
  memcpy((void*)&(nodedata.saddr), (const void*)saddr, sizeof(struct sockaddr));
  nodedata.connect_fd   = new_socket_fd;
  nodedata.socket_bufev = bufferevent_socket_new(tcp_monitor_event_arg.tcp_eventbase, \
                                                 new_socket_fd, \
                                                 BEV_OPT_CLOSE_ON_FREE);
  if(!nodedata.socket_bufev){
    fprintf(stderr, "bufferevent create failed!\n");
  }
  bufferevent_setcb(nodedata.socket_bufev, \
                    bev_tcp_monitor_read_cb, \
                    NULL, \
                    bev_tcp_monitor_event_cb, \
                    NULL);
  bufferevent_set_timeouts(nodedata.socket_bufev, \
                           &timeout_read, \
                           &timeout_write);
  bufferevent_setwatermark(nodedata.socket_bufev, \
                           EV_READ , \
                           CFG_TCP_MONITOR_READ_LOWMARK, \
                           CFG_TCP_MONITOR_READ_HIGHMARK);
  bufferevent_enable(nodedata.socket_bufev, EV_READ|EV_WRITE);
  //TCP_MONITOR连接信息插入链表
  list_insert_nodedata(&tcp_monitor_head, 1, nodedata);
  //客户端连接日志信息记录
  log_handle(LOG_TYPE_CONNECT, new_socket_fd, CFG_TCP_MONITOR_PORT, "connect success.", 0);
}

void listen_event_handle(void)
{
  struct event_base*      listen_base;
  struct evconnlistener*  listener;
  struct sockaddr_in      sock_in;

  listen_base = listen_event_arg.listen_base;
  listen_base = event_base_new();
  if(!listen_base){
    fprintf(stderr, "could not initialize libevent!\n");
    exit(-1);
  }

  /* HTTP监听 */
  memset(&sock_in, 0, sizeof(sock_in));
  sock_in.sin_family      = AF_INET;
  sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_in.sin_port        = htons(CFG_HTTP_PORT);
  listener = evconnlistener_new_bind(listen_base, listener_http_cb, \
                                     (char*)"hello http", \
                                     LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1, \
                                     (struct sockaddr*)&sock_in, sizeof(sock_in) );
  if(!listener){
    fprintf(stderr, "could not create a listener on port: %d!\n", CFG_HTTP_PORT);
    exit(-1);
  }

  /* TCP_DEV监听 */
  memset(&sock_in, 0, sizeof(sock_in));
  sock_in.sin_family      = AF_INET;
  sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_in.sin_port        = htons(CFG_TCP_DEV_PORT);
  listener = evconnlistener_new_bind(listen_base, listener_tcp_dev_cb, \
                                     (char*)"hello tcp dev", \
                                     LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1, \
                                     (struct sockaddr*)&sock_in, sizeof(sock_in) );
  if(!listener){
    fprintf(stderr, "could not create a listener on port: %d!\n", CFG_TCP_DEV_PORT);
    exit(-1);
  }

  /* TCP_MONITOR监听 */
  memset(&sock_in, 0, sizeof(sock_in));
  sock_in.sin_family      = AF_INET;
  sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_in.sin_port        = htons(CFG_TCP_MONITOR_PORT);
  listener = evconnlistener_new_bind(listen_base, listener_tcp_monitor_cb, \
                                     (char*)"hello tcp monitor", \
                                     LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1, \
                                     (struct sockaddr*)&sock_in, sizeof(sock_in) );
  if(!listener){
    fprintf(stderr, "could not create a listener on port: %d!\n", CFG_TCP_MONITOR_PORT);
    exit(-1);
  }

  event_base_dispatch(listen_base);
  evconnlistener_free(listener);
  event_base_free(listen_base);
}
