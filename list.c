#define EXT_LIST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

listStatus  listCreate(head_t **head, uint32_t n)
{
  int i;
  node_t *p = NULL;
  node_t *r = NULL;

  *head = (head_t*)malloc(sizeof(head_t));
  if(*head == NULL)return ERROR;

  (*head) ->length = 0;
  r = (*head) ->node;
  for(i = 0; i < n; i++)
  {
    p = (node_t*)malloc(sizeof(node_t));
    if(p == NULL)return ERROR;
    //----------节点数据赋值开始----------
    //p->data = dummy;//节点内容赋值
    //----------节点数据赋值结束----------
    r ->next = p;
    r        = p;
    (*head) ->length++;
  }
  r->next = NULL;
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
  node_t      *p   = NULL;
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
  node_t      *p   = NULL;
  uint32_t    i;

  //传参校验
  if(index > head ->length)return ERROR;

  p = head ->node ->next;
  for(i = 0; i < index; i++){
    p = p ->next;
  }
  //元素赋值
  *nodeData = p ->data;
  
  return SUCCESS;
}

listStatus  listCheckNodeData(head_t *head, uint32_t *index, nodeData_t nodeData)
{
  node_t *p   = NULL;
  uint32_t    cnt = 0;

  p = head ->node ->next;
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

listStatus  listInsertNodeData(head_t *head, uint32_t index, nodeData_t nodeData)
{
  node_t *p   = NULL;
  uint32_t    length;
  uint32_t    i;

  //传参校验
  listLength(head, &length);
  if(index > length)return ERROR;

  p = head ->node ->next;
  for(i = 0; i < index; i++){
    p = p ->next;
  }
  memcpy((void*)p, (const void*)&nodeData, sizeof(nodeData_t));

  return SUCCESS;
}
listStatus  listDeleteNodeData(head_t *head, uint32_t index, nodeData_t *nodeData)
{
  node_t *p   = NULL;
  uint32_t    length;
  uint32_t    i;

  //传参校验
  listLength(head, &length);
  if(index > length)return ERROR;

  p = head ->node ->next;
  for(i = 0; i < index; i++){
    p = p ->next;
  }
}
