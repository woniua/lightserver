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

/* MYSQL连接信息 */
#define  CFG_MYSQL_HOST                          "localhost"
#define  CFG_MYSQL_USER                          "root"
#define  CFG_MYSQL_PASSWORD                      "123456"
#define  CFG_MYSQL_DATABASE                      "lightserver"

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
//TCP_DEV数据发送缓冲区长度(字节)
#define  CFG_TCP_DEV_WRITE_BUF_LEN               1024
//TCP_MONITOR数据接收缓冲区长度(字节)
#define  CFG_TCP_MONITOR_READ_BUF_LEN            1024
//TCP_MONITOR数据发送缓冲区长度(字节)
#define  CFG_TCP_MONITOR_WRITE_BUF_LEN           1024

/* 并发数限制 */
//web端HTTP访问并发数限制
#define  CFG_HTTP_CONCURRENCY_NUM                100
//实际的TCP终端运行设备并发数限制
#define  CFG_TCP_DEV_CONCURRENCY_NUM             1000
//CS架构的终端监控软件(如桌面软件,APP等)并发数限制
#define  CFG_TCP_MONITOR_CONCURRENCY_NUM         100

/* 读水位配置 */
//当bufferevent从socket接收到的数据字节数达到或超过此值时，将出发读数据回调函数，0表示有数据
//立即触发读数据回调函数
#define  CFG_TCP_DEV_READ_LOWMARK                5
#define  CFG_TCP_MONITOR_READ_LOWMARK            5
//当socket接收数据的速度大于读取数据的速度时，缓冲区剩余的字节数大于此值时，bufferevent将停止
//从socket获取数据，低于此值时继续从socket获取数据，0表示此值无限大，bufferevent不会停止从
//soket获取数据
#define  CFG_TCP_DEV_READ_HIGHMARK               0
#define  CFG_TCP_MONITOR_READ_HIGHMARK           0

/*----------------------------------功能配置项----------------------------------*/
/* 身份认证支持 */
//身份认证支持被打开后,客户端成功连接到服务器后发送的第一帧数据被认为是身份认证数据帧,客户端的身
//份信息以字符串的形式表示,存储于链表中的name元素中, 0:不支持; 1:支持
#define  CFG_EN_TCP_DEV_ID_AUTH                  0
#define  CFG_EN_TCP_MONITOR_ID_AUTH              0

/* 日志记录支持 */
//日志信息打印到屏幕，对所有日志类型有效，0:不支持; 1:支持
#define  CFG_EN_TCP_DEV_LOG_PRINT                1
#define  CFG_EN_TCP_MONITOR_LOG_PRINT            1
//日志信息记录到文件，对所有日志类型有效，0:不支持; 1:支持
#define  CFG_EN_TCP_DEV_LOG_RECORD               1
#define  CFG_EN_TCP_MONITOR_LOG_RECORD           1
//网络客户端连接到服务器的信息记录，0:不支持; 1:支持
#define  CFG_EN_TCP_DEV_LOG_TYPE_CONNECT         1
#define  CFG_EN_TCP_MONITOR_LOG_TYPE_CONNECT     1
//网络客户端从服务器断开连接的信息记录，0:不支持; 1:支持
#define  CFG_EN_TCP_DEV_LOG_TYPE_DISCONNECT      1
#define  CFG_EN_TCP_MONITOR_LOG_TYPE_DISCONNECT  1
//网络客户端的接收信息以字符串的形式记录，0:不支持; 1:支持
//非调试状态建议关闭。
#define  CFG_EN_TCP_DEV_LOG_TYPE_RECV_STRING     1
#define  CFG_EN_TCP_MONITOR_LOG_TYPE_RECV_STRING 1
//网络客户端的发送信息以字符串的形式记录，0:不支持; 1:支持
//非调试状态建议关闭。
#define  CFG_EN_TCP_DEV_LOG_TYPE_SEND_STRING     1
#define  CFG_EN_TCP_MONITOR_LOG_TYPE_SEND_STRING 1
//网络客户端的接收信息以十六进制的形式记录，0:不支持; 1:支持
//非调试状态建议关闭。
#define  CFG_EN_TCP_DEV_LOG_TYPE_RECV_HEX        1
#define  CFG_EN_TCP_MONITOR_LOG_TYPE_RECV_HEX    1
//网络客户端的发送信息以十六进制的形式记录，0:不支持; 1:支持
//非调试状态建议关闭。
#define  CFG_EN_TCP_DEV_LOG_TYPE_SEND_HEX        1
#define  CFG_EN_TCP_MONITOR_LOG_TYPE_SEND_HEX    1
//服务器运行的错误的信息记录，0:不支持; 1:支持
#define  CFG_EN_TCP_DEV_LOG_TYPE_ERROR           1
#define  CFG_EN_TCP_MONITOR_LOG_TYPE_ERROR       1

/* OpenSSL传输加密支持 */
//TCP socket的OpenSSL的加密传输,0:不支持; 1:支持
#define  CFG_EN_HTTP_OPENSSL                     1
#define  CFG_EN_TCP_DEV_OPENSSL                  1
#define  CFG_EN_TCP_MONITOR_OPENSSL              1

/* TCP_DEV的mysql数据库支持 */
//dev发送来的数据存储进mysql数据库,0:不支持; 1:支持
#define  CFG_EN_TCP_DEV_MYSQL                    1

/* 数据回显支持 */
//TCP_DEV的设备向服务器发送完一帧数据后，服务器可以马上响应一帧数据到该设备,0:不支持; 1:支持
#define  CFG_EN_TCP_DEV_ACK                      1
//TCP_MONITOR的设备向服务器发送完一帧数据后，服务器可以马上响应一帧数据到该设备,0:不支持; 1:支持
#define  CFG_EN_TCP_MONITOR_ACK                  1

/* 数据转发支持 */
//TCP_DEV的设备向服务器发送完一帧数据后，服务器可以马上将该数据原样的或经过处理后发送到TCP_DEV
//的某台或某几台在线设备,0:不支持; 1:支持
#define  CFG_EN_TCP_DEV_FORWARD_DEV              1
//TCP_DEV的设备向服务器发送完一帧数据后，服务器可以马上将该数据原样的或经过处理后发送到TCP_MONITOR
//的某台或某几台在线设备,0:不支持; 1:支持
#define  CFG_EN_TCP_DEV_FORWARD_MONITOR          1
//TCP_DEV的设备向服务器发送完一帧数据后，服务器可以马上将该数据原样的或经过处理后向TCP_DEV的其
//它所有在线设备进行广播,0:不支持; 1:支持
#define  CFG_EN_TCP_DEV_BROADCAST_DEV            1
//TCP_DEV的设备向服务器发送完一帧数据后，服务器可以马上将该数据原样的或经过处理后向TCP_MONITOR
//的其它在线所有设备进行广播,0:不支持; 1:支持
#define  CFG_EN_TCP_DEV_BROADCAST_MONITOR        1
//TCP_MONITOR的设备向服务器发送完一帧数据后，服务器可以马上将该数据原样的或经过处理后发送到TCP_MONITOR
//的某台或某几台在线设备,0:不支持; 1:支持
#define  CFG_EN_TCP_MONITOR_FORWARD_MONITOR      1
//TCP_MONITOR的设备向服务器发送完一帧数据后，服务器可以马上将该数据原样的或经过处理后发送到TCP_DEV
//的某台或某几台在线设备,0:不支持; 1:支持
#define  CFG_EN_TCP_MONITOR_FORWARD_DEV          1
//TCP_MONITOR的设备向服务器发送完一帧数据后，服务器可以马上将该数据原样的或经过处理后向TCP_MONITOR
//的其它所有在线设备进行广播,0:不支持; 1:支持
#define  CFG_EN_TCP_MONITOR_BROADCAST_MONITOR    1
//TCP_MONITOR的设备向服务器发送完一帧数据后，服务器可以马上将该数据原样的或经过处理后向TCP_DEV
//的其它所有在线设备进行广播,0:不支持; 1:支持
#define  CFG_EN_TCP_MONITOR_BROADCAST_DEV        1

/* 数据推送支持 */
//服务器监测到某触发条件被满足后向TCP_DEV的某台在线设备推送数据,0:不支持; 1:支持
#define  CFG_EN_SERVER_FORWARD_DEV               1
//服务器监测到某触发条件被满足后向TCP_MONITOR的某台在线设备推送数据,0:不支持; 1:支持
#define  CFG_EN_SERVER_FORWARD_MONITOR           1
//服务器监测到某触发条件被满足后向TCP_DEV的所有在线设备推送数据,0:不支持; 1:支持
#define  CFG_EN_SERVER_BROADCAST_DEV             1
//服务器监测到某触发条件被满足后向TCP_MONITOR的所有在线设备推送数据,0:不支持; 1:支持
#define  CFG_EN_SERVER_BROADCAST_MONITOR         1

#endif /* #ifndef __CONFIG_H */
