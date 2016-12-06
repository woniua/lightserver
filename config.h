#ifndef __CONFIG_H
#define __CONFIG_H

/* 版本号宏定义 */
#define  MAJOR_VERSION                           0//主版本号
#define  MINOR_VERSION                           1//次版本号
#define  REVISION_VERSION                        1//修订版本号


/* 端口号宏定义 */
#define  CFG_HTTP_PORT                           8080
#define  CFG_TCP_PORT                            8899

/* TCP读写超时时间 */
#define  CFG_TIMEOUT_READ_S                      5//秒
#define  CFG_TIMEOUT_READ_US                     0//微秒
#define  CFG_TIMEOUT_WRITE_S                     5//秒
#define  CFG_TIMEOUT_WRITE_US                    0//微秒

#define  CFG_SYS_READ_LOWMARK                    5
#define  CFG_SYS_READ_HIGHMARK                   0//对于高水位，0表示无限大
#define  CFG_SYS_WRITE_LOWMARK                   0
#define  CFG_SYS_WRITE_HIGHMARK                  0//对于高水位，0表示无限大

#endif /* #ifndef __CONFIG_H */
