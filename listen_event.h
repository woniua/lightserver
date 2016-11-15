#ifndef __LISTEN_EVENT_H
#define __LISTEN_EVENT_H

#ifdef EXT_LISTEN_EVENT
#define EXT_LISTEN_EVENT
#else
#define EXT_LISTEN_EVENT extern
#endif

/* 头文件包含 */
#include <stdint.h>
#include <event2/event.h>
#include <event2/listener.h>

typedef struct{
  struct event_base*      listen_base;//监听base
}listen_event_arg_t;
EXT_LISTEN_EVENT listen_event_arg_t listen_event_arg;

/* 公有函数声明 */
EXT_LISTEN_EVENT void listen_event_handle(void);

#endif /* #ifndef __LISTEN_EVENT_H */
