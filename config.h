#ifndef __CONFIG_H
#define __CONFIG_H

/* 版本号 */
#define  MAJOR_VERSION                           0//主版本号
#define  MINOR_VERSION                           1//次版本号
#define  REVISION_VERSION                        1//修订版本号

/* 监听端口号 */
#define  CFG_HTTP_PORT                           8080
#define  CFG_TCP_DEV_PORT                        8899
#define  CFG_TCP_MONITOR_PORT                    8900

/* TCP_DEV socket读写超时时间 */
#define  CFG_TCP_DEV_TIMEOUT_READ_S              5//秒
#define  CFG_TCP_DEV_TIMEOUT_READ_US             0//微秒
#define  CFG_TCP_DEV_TIMEOUT_WRITE_S             5//秒
#define  CFG_TCP_DEV_TIMEOUT_WRITE_US            0//微秒

/* TCP_MONITOR socket读写超时时间 */
#define  CFG_TCP_MONITOR_TIMEOUT_READ_S          5//秒
#define  CFG_TCP_MONITOR_TIMEOUT_READ_US         0//微秒
#define  CFG_TCP_MONITOR_TIMEOUT_WRITE_S         5//秒
#define  CFG_TCP_MONITOR_TIMEOUT_WRITE_US        0//微秒

/* TCP数据接收缓冲区长度 */
//TCP_DEV数据接收缓冲区长度(字节)
#define  CFG_TCP_DEV_READ_BUF_LEN                1024
//TCP_MONITOR数据接收缓冲区长度(字节)
#define  CFG_TCP_MONITOR_READ_BUF_LEN            1024

/* 并发数限制 */
//web端HTTP访问并发数限制
#define  CFG_HTTP_CONCURRENCY_NUM                1000
//实际的TCP终端运行设备并发数限制
#define  CFG_TCP_DEV_CONCURRENCY_NUM             1000
//CS架构的终端监控软件(如桌面软件,APP等)并发数限制
#define  CFG_TCP_MONITOR_CONCURRENCY_NUM         100

/* 读水位配置 */
//当bufferevent从socket接收到的数据字节数达到或超过此值时，将出发读数据回调函数，0表示有数据
//立即触发读数据回调函数
#define  CFG_SYS_TCP_DEV_READ_LOWMARK            5
#define  CFG_SYS_TCP_MONITOR_READ_LOWMARK        5
//当socket接收数据的速度大于读取数据的速度时，缓冲区剩余的字节数大于此值时，bufferevent将停止
//从socket获取数据，低于此值时继续从socket获取数据，0表示此值无限大，bufferevent不会停止从
//soket获取数据
#define  CFG_SYS_TCP_DEV_READ_HIGHMARK           0
#define  CFG_SYS_TCP_MONITOR_READ_HIGHMARK       0

/* OpenSSL传输加密支持 */
//TCP socket的OpenSSL的加密传输,0:不支持; 1:支持
#define  CFG_EN_HTTP_OPENSSL                     1
#define  CFG_EN_TCP_DEV_OPENSSL                  1
#define  CFG_EN_TCP_MONITOR_OPENSSL              1

/* TCP_DEV的mysql数据库支持 */
//dev发送来的数据存储进mysql数据库,0:不支持; 1:支持
#define  CFG_EN_TCP_DEV_MYSQL                    1
//#define  

/*  */

#endif /* #ifndef __CONFIG_H */
