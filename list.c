#define EXT_LIST

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

listStatus  listCreate(listNode_t **list, uint32_t n)
{
  int i;
  listNode_t *p = NULL;
  listNode_t *r = NULL;

  *list = (listNode_t*)malloc(sizeof(listNode_t));
  if(*list == NULL)return ERROR;
  r = *list;

  for(i = 0; i < n; i++)
  {
    p = (listNode_t*)malloc(sizeof(listNode_t));
    if(p == NULL)return ERROR;
    //----------节点内容赋值开始----------
    //p->client = dummy;//节点内容赋值
    //----------节点内容赋值结束----------
    r ->next = p;
    r = p;
  }
  r->next = NULL;
  return SUCCESS;
}

listStatus  listClear(listNode_t *list)
{
  listNode_t *p = NULL;
  listNode_t *q = NULL;

  p = list ->next;
  while(p)
  {
    q = p ->next;
    free(p);
    p = q;
  }
  list ->next = NULL;
  return SUCCESS;
}

listStatus  listIsEmpty(listNode_t *list)
{
  if(list ->next == NULL)//链表为空
    return SUCCESS;
  else
    return ERROR;
}

listStatus  listLength(listNode_t *list, uint32_t *length)
{
  listNode_t *p   = NULL;
  uint32_t    cnt = 0;

  p = list ->next;
  while(p)
  {
    p = p ->next;
    cnt++;
  }
  *length = cnt;
  return SUCCESS;
}

listStatus  listGetElem(listNode_t *list, uint32_t index, clientInfo_t *element)
{
  listNode_t *p   = NULL;
  uint32_t    length;
  uint32_t    i;

  //传参校验
  listLength(list, &length);
  if(index > length)return ERROR;

  p = list ->next;
  for(i = 0; i < index; i++){
    p = p ->next;
  }
  //元素赋值
  *element = p ->client;
  return SUCCESS;
}

listStatus  listCheckElem(listNode_t *list, uint32_t *index, clientInfo_t element)
{
  listNode_t *p   = NULL;
  uint32_t    cnt = 0;

  p = list ->next;
  while(p)
  {
    //判断给出的元素于链表当前节点的值是否相等
    if(1){
      *index = cnt;
      return SUCCESS;
    }
    p = p ->next;
    cnt++;
  }

  return ERROR;
}

listStatus  listInsertElem(listNode_t *list, uint32_t index, clientInfo_t element)
{
  listNode_t *p   = NULL;
  uint32_t    length;
  uint32_t    i;

  //传参校验
  listLength(list, &length);
  if(index > length)return ERROR;

}
listStatus  listDeleteElem(listNode_t *list, uint32_t index, clientInfo_t *element)
{

}
