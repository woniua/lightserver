#ifndef __LIST_H
#define __LIST_H

#ifdef  EXT_LIST
#define EXT_LIST
#else
#define EXT_LIST extern
#endif

/* 头文件包含 */
#include <stdint.h>
#include <netinet/in.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <mysql/mysql.h>

/* 返回状态枚举值 */
typedef enum{ERROR= 0, SUCCESS= !ERROR}listStatus;

/* 节点结构体数据域 */
typedef struct{
  int32_t              connect_fd;    //连接描述符
  struct sockaddr_in   saddr;         //连接的IP地址和端口信息
  char*                name;          //连接的别名
  struct event*        socket_ev;     //当前连接注册的事件指针
  struct bufferevent*  socket_bufev;  //当前连接注册bufferevent
  MYSQL*               mysql_fd;      //指向MYSQL对象的指针
}nodedata_t;

/* 链表节点结构体 */
typedef struct node{
  nodedata_t    data;
  struct node*  next;
}node_t;

/* 定义链表头结点 */
typedef struct{
  node_t*   node;
  int32_t   length;    //链表长度
}head_t;
EXT_LIST head_t http_head;
EXT_LIST head_t tcp_dev_head;
EXT_LIST head_t tcp_monitor_head;

/* 公有函数声明 */
EXT_LIST  listStatus  list_create(head_t* head, uint32_t n);  //创建一个新的线性表
EXT_LIST  void        list_clear(head_t* head);               //清空线性表
EXT_LIST  listStatus  list_is_empty(head_t* head);            //检查线性表是否为空
EXT_LIST  uint32_t    list_get_length(head_t* head);          //获取线性表的长度
//获取线性表第i位置的元素(首个元素索引值为1)
EXT_LIST  listStatus  list_get_nodedata(head_t* head, uint32_t index, nodedata_t* nodedata);
//检查线性表中指定连接描述符是否存在并返回元素的位置和元素内容，如元素不存在返回-1(首个元素索引值为1)
EXT_LIST  int32_t     list_check_nodedata(head_t* head, int32_t connect_fd, nodedata_t* nodedata);
//向线性表中指定节点之前插入一个元素(首个元素索引值为1)
EXT_LIST  listStatus  list_insert_nodedata(head_t* head, uint32_t index, nodedata_t nodedata);
//从线性表中删除一个元素并取出删除元素的数据(首个元素索引值为1)
EXT_LIST  listStatus  list_delete_nodedata(head_t* head, uint32_t index, nodedata_t* nodedata);

#endif /* #ifndef __LIST_H */
