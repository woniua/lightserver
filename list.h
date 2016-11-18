#ifndef __LIST_H
#define __LIST_H

#ifdef EXT_LIST
#define EXT_LIST
#else
#define EXT_LIST extern
#endif

/* 头文件包含 */
#include <stdint.h>

/* 返回状态枚举值 */
typedef enum{ERROR= 0, SUCCESS= !ERROR}listStatus;

/* 节点结构体数据域 */
typedef struct{
  int a;
}nodeData_t;

/* 链表节点结构体 */
typedef struct node{
  nodeData_t    data;
  struct node   *next;
}node_t;

/* 定义链表头结点 */
typedef struct{
  int       length;    //链表长度
  node_t    *node;
}head_t;
EXT_LIST head_t *head;

/* 公有函数声明 */
EXT_LIST  listStatus  listCreate(head_t **head, uint32_t n);    //创建一个新的线性表
EXT_LIST  listStatus  listClear(head_t *head);   //清空线性表
EXT_LIST  listStatus  listIsEmpty(head_t *head); //检查线性表是否为空
EXT_LIST  listStatus  listLength(head_t *head, uint32_t *length); //获取线性表的长度
EXT_LIST  listStatus  listGetNodeData(head_t *head, uint32_t index, nodeData_t *nodeData); //获取线性表第i位置的元素
EXT_LIST  listStatus  listCheckNodeData(head_t *head, uint32_t *index, nodeData_t nodeData);//检查线性表中指定元素是否存在并返回元素的位置
EXT_LIST  listStatus  listInsertNodeData(head_t *head, uint32_t index, nodeData_t nodeData);//向线性表中插入一个元素
EXT_LIST  listStatus  listDeleteNodeData(head_t *head, uint32_t index, nodeData_t *nodeData);//从线性表中删除一个元素

#endif /* #ifndef __LIST_H */
