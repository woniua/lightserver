#ifndef __HTTP_EVENT_H
#define __HTTP_EVENT_H

#ifdef EXT_HTTP_EVENT
#define EXT_HTTP_EVENT
#else
#define EXT_HTTP_EVENT extern
#endif

/* 头文件包含 */
#include <stdint.h>
#include <event2/event.h>
#include <event2/listener.h>

typedef struct{
  struct event_base*      eventbase;//http事件base

}http_event_arg_t;
EXT_HTTP_EVENT http_event_arg_t http_event_arg;

/* 公有函数声明 */
EXT_HTTP_EVENT void http_event_thread_create(evutil_socket_t socket_fd);

#endif /* #ifndef __HTTP_EVENT_H */
