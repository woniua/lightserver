目录
====================
1.为什么lightserver
2.lightserver提供的功能


1.为什么lightserver
====================
对于中小型的物联网数据采集，服务器端业务层之外的网络连接处理逻辑几乎没有变化。
lightserver提供公共的网络层处理逻辑，使得开发人员将更多经历放在业务层的数据处理上。


2.lightserver提供的功能
====================
lightserver提供以下功能：
- 轻量的转发服务器
- 轻量的回显服务器
- client活动日志记录
- 简单的web端client连接监控与查看
lightserver采用libevent作为网络驱动库，http协议处理采用http-parser，设计client端最
大并发数10000连接，web端访问最大并发数1000连接。设计架构采用libevent+多线程。
