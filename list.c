#define EXT_LIST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

listStatus  listCreate(head_t *head, uint32_t n)
{
  int    i;
  node_t *p = NULL;
  node_t *r = NULL;

  head ->node = (node_t*)malloc(sizeof(node_t));
  if(head ->node == NULL)return ERROR;
  head ->length = 0;
  r = head ->node;
  for(i = 0; i < n; i++)
  {
    p = (node_t*)malloc(sizeof(node_t));
    if(p == NULL)return ERROR;

    //----------节点数据赋值开始----------
    // static int cnt  =123;
    // static int cnt1 =12;
    // static int cnt2 =23;
    // p->data.a = cnt++;//节点内容赋值
    // p->data.b = cnt1++;
    // p->data.c = cnt2++;
    //----------节点数据赋值结束----------
    r ->next = p;
    r        = p;
    head ->length++;
  }
  r ->next = NULL;
  return SUCCESS;
}

listStatus  listClear(head_t *head)
{
  node_t *p = NULL;
  node_t *q = NULL;

  p = head ->node ->next;
  while(p)
  {
    q = p ->next;
    free(p);
    p = q;
    head ->length--;
  }
  head ->node ->next = NULL;
  return SUCCESS;
}

listStatus  listIsEmpty(head_t *head)
{
  if(head ->node ->next == NULL)//链表为空
    return SUCCESS;
  else
    return ERROR;
}

listStatus  listLength(head_t *head, uint32_t *length)
{
  node_t      *p  = NULL;
  uint32_t    cnt = 0;

  p = head ->node ->next;
  while(p)
  {
    p = p ->next;
    cnt++;
  }
  *length = cnt;
  return SUCCESS;
}

listStatus  listGetNodeData(head_t *head, uint32_t index, nodeData_t *nodeData)
{
  node_t      *p = NULL;
  uint32_t    i;

  //传参校验
  if( (index > head ->length)||(index < 1) )return ERROR;

  p = head ->node;
  for(i = 0; i < index; i++){
    p = p ->next;
  }
  //元素赋值
  memcpy((void*)nodeData, (const void*)&(p ->data), sizeof(nodeData_t));
  return SUCCESS;
}

listStatus  listCheckNodeData(head_t *head, uint32_t *index, nodeData_t nodeData)
{
  node_t      *p  = NULL;
  uint32_t    cnt = 1;
  int32_t     result;

  p = head ->node ->next;
  while(p)
  {
    //判断给出的元素与链表当前节点数据区的值是否相等
    result = memcmp((const void*)&nodeData, (const void*)&(p ->data), sizeof(nodeData_t));
    if(!result){
      *index = cnt;
      return SUCCESS;
    }
    p = p ->next;
    cnt++;
  }

  return ERROR;
}

listStatus  listInsertNodeData(head_t *head, uint32_t index, nodeData_t nodeData)
{
  node_t      *p = NULL;
  node_t      *s = NULL;
  uint32_t    length;
  uint32_t    i;

  //传参校验
  if( (index > (head->length + 1))||(index < 1) )return ERROR;

  p = head ->node;
  for(i = 0; i < (index-1); i++){
    p = p ->next;
  }
  s = (node_t*)malloc(sizeof(node_t));
  if(s == NULL)return ERROR;
  memcpy((void*)&(s ->data), (const void*)&nodeData, sizeof(nodeData_t));
  s ->next = p ->next;
  p ->next = s;
  head ->length++;

  return SUCCESS;
}

listStatus  listDeleteNodeData(head_t *head, uint32_t index, nodeData_t *nodeData)
{
  node_t      *p = NULL;
  node_t      *q = NULL;
  uint32_t    length;
  uint32_t    i;

  //传参校验
  if( (index > head ->length)||(index < 1) )return ERROR;

  p = head ->node;
  for(i = 0; i < (index-1); i++){
    p = p ->next;
  }
  q        = p ->next;
  p ->next = q ->next;
  free(q);
  head ->length--;

  return SUCCESS;
}
