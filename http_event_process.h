#ifndef __HTTP_EVENT_PROCESS_H
#define __HTTP_EVENT_PROCESS_H

#ifdef EXT_HTTP_EVENT_PROCESS
#define EXT_HTTP_EVENT_PROCESS
#else
#define EXT_HTTP_EVENT_PROCESS extern
#endif

/* 头文件包含 */
#include <stdint.h>

typedef struct{

}http_event_arg_t;
EXT_HTTP_EVENT_PROCESS http_event_arg_t http_event_arg;

/* 公有函数声明 */
EXT_HTTP_EVENT_PROCESS void* http_event_process(void* arg);

#endif /* #ifndef __HTTP_EVENT_PROCESS_H */
