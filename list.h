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

/* 客户端信息结构体 */
typedef struct{
  int a;
}clientInfo_t;

/* 链表节点结构体 */
typedef struct node {
  clientInfo_t   client;
  struct node   *next;
}listNode_t;

/* 定义链表指针 */
EXT_LIST listNode_t *list;

/* 公有函数声明 */
EXT_LIST  listStatus  listCreate(listNode_t **list, uint32_t n);    //创建一个新的线性表
EXT_LIST  listStatus  listClear(listNode_t *list);   //清空线性表
EXT_LIST  listStatus  listIsEmpty(listNode_t *list); //检查线性表是否为空
EXT_LIST  listStatus  listLength(listNode_t *list, uint32_t *length); //获取线性表的长度
EXT_LIST  listStatus  listGetElem(listNode_t *list, uint32_t index, clientInfo_t *element); //获取线性表第i位置的元素
EXT_LIST  listStatus  listCheckElem(listNode_t *list, uint32_t *index, clientInfo_t element);//检查线性表中指定元素是否存在并返回元素的位置
EXT_LIST  listStatus  listInsertElem(listNode_t *list, uint32_t index, clientInfo_t element);//向线性表中插入一个元素
EXT_LIST  listStatus  listDeleteElem(listNode_t *list, uint32_t index, clientInfo_t *element);//从线性表中删除一个元素

#endif /* #ifndef __LIST_H */
