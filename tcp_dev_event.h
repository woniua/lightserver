#ifndef __TCP_DEV_EVENT_H
#define __TCP_DEV_EVENT_H

#ifdef EXT_TCP_DEV_EVENT
#define EXT_TCP_DEV_EVENT
#else
#define EXT_TCP_DEV_EVENT extern
#endif

/* 头文件包含 */
#include <stdint.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>

typedef struct{
  struct event_base*      tcp_eventbase;//tcp读事件base

}tcp_dev_event_arg_t;
EXT_TCP_DEV_EVENT tcp_dev_event_arg_t tcp_dev_event_arg;

/* 公有函数声明 */
EXT_TCP_DEV_EVENT void tcp_dev_event_thread_create(evutil_socket_t socket_fd);
// TCP_DEV读数据回调函数
EXT_TCP_DEV_EVENT void bev_tcp_dev_read_cb( struct bufferevent* bev, void* ctx);
// TCP_DEV连接异常回调函数
EXT_TCP_DEV_EVENT void bev_tcp_dev_event_cb(struct bufferevent* bev, short events, void* ctx);

#endif /* #ifndef __TCP_DEV_EVENT_H */
