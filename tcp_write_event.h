#ifndef __TCP_WRITE_EVENT_H
#define __TCP_WRITE_EVENT_H

#ifdef EXT_TCP_WRITE_EVENT
#define EXT_TCP_WRITE_EVENT
#else
#define EXT_TCP_WRITE_EVENT extern
#endif

/* 头文件包含 */
#include <stdint.h>
#include <event2/event.h>
#include <event2/listener.h>

typedef struct{
  struct event_base*      tcp_write_base;//tcp写事件base

}tcp_write_event_arg_t;
EXT_TCP_WRITE_EVENT tcp_write_event_arg_t tcp_write_event_arg;

/* 公有函数声明 */
EXT_TCP_WRITE_EVENT void tcp_write_event_thread_create(evutil_socket_t socket_fd);

#endif /* #ifndef __TCP_WRITE_EVENT_H */
