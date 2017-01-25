#ifndef __PORTABLE_H
#define __PORTABLE_H

#ifdef  EXT_PORTABLE
#define EXT_PORTABLE
#else
#define EXT_PORTABLE extern
#endif

/* 头文件包含 */
#include <stdint.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

/* 公有函数声明 */
//config.h中CFG_EN_TCP_DEV_ACK设置为1时此函数有效
EXT_PORTABLE  void  portable_tcp_dev_ack_handle(const uint8_t* read_buffer, \
                                                const uint32_t read_length, \
                                                uint8_t*  write_buffer, \
                                                uint32_t* write_length);

//config.h中CFG_EN_TCP_MONITOR_ACK设置为1时此函数有效
EXT_PORTABLE  void  portable_tcp_monitor_ack_handle(const uint8_t* read_buffer, \
                                                    const uint32_t read_length, \
                                                    uint8_t*  write_buffer, \
                                                    uint32_t* write_length);
#endif /* #ifndef __PORTABLE_H */
