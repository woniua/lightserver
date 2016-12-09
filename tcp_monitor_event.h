#ifndef __TCP_MONITOR_EVENT_H
#define __TCP_MONITOR_EVENT_H

#ifdef EXT_TCP_MONITOR_EVENT
#define EXT_TCP_MONITOR_EVENT
#else
#define EXT_TCP_MONITOR_EVENT extern
#endif

/* 头文件包含 */
#include <stdint.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>

typedef struct{
  struct event_base*      tcp_eventbase;//tcp读事件base

}tcp_monitor_event_arg_t;
EXT_TCP_MONITOR_EVENT tcp_monitor_event_arg_t tcp_monitor_event_arg;

/* 公有函数声明 */
EXT_TCP_MONITOR_EVENT void tcp_monitor_event_thread_create(evutil_socket_t socket_fd);
// TCP读数据回调函数
EXT_TCP_MONITOR_EVENT void bev_tcp_monitor_read_cb( struct bufferevent *bev, void *ctx);
// TCP连接异常回调函数
EXT_TCP_MONITOR_EVENT void bev_tcp_monitor_event_cb(struct bufferevent *bev, short events, void *ctx);

#endif /* #ifndef __TCP_MONITOR_EVENT_H */
