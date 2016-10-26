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
    //p->client = dummy;//节点内容赋值
    r -> next = p;
    r = p;
  }
  r->next = NULL;
  return SUCCESS;
}

listStatus  listClear(listNode_t *list)
{
  listNode_t *p = NULL;
  listNode_t *q = NULL;

  p = list -> next;
  while(p)
  {
    q = p -> next;
    free(p);
    p = q;
  }
  list -> next = NULL;
  return SUCCESS;
}

listStatus  listIsEmpty(listNode_t *list)
{
  if(list -> next == NULL)//链表为空
    return SUCCESS;
  else
    return ERROR;
}

listStatus  listLength(listNode_t *list, uint32_t *length)
{
  listNode_t *p   = NULL;
  uint32_t    cnt = 0;
  p = list -> next;
  while(p)
  {
    p = p -> next;
    cnt++;
  }
  *length = cnt;
  return SUCCESS;
}

listStatus  listGetElem(listNode_t *list, uint32_t index, clientInfo_t *element)
{

}

listStatus  listCheckElem(listNode_t *list, uint32_t *index, clientInfo_t element)
{

}

listStatus  listInsertElem(listNode_t *list, uint32_t index, clientInfo_t element)
{

}
listStatus  listDeleteElem(listNode_t *list, uint32_t index, clientInfo_t *element)
{

}
