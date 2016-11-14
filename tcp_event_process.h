#ifndef __TCP_EVENT_PROCESS_H
#define __TCP_EVENT_PROCESS_H

#ifdef EXT_TCP_EVENT_PROCESS
#define EXT_TCP_EVENT_PROCESS
#else
#define EXT_TCP_EVENT_PROCESS extern
#endif

/* 头文件包含 */
#include <stdint.h>

typedef struct{

}tcp_event_arg_t;
EXT_TCP_EVENT_PROCESS tcp_event_arg_t tcp_event_arg;

/* 公有函数声明 */
EXT_TCP_EVENT_PROCESS void* tcp_event_process(void* arg);

#endif /* #ifndef __TCP_EVENT_PROCESS_H */
