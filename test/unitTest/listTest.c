#define EXT_LIST

#include <stdio.h>
#include <unistd.h>

#include "../../list.h"

int main(int argc, char const *argv[])
{
  uint32_t     len = 0;
  uint32_t     index =0;
  nodeData_t   nodedata={0};
  listCreate(&head, 10);
  listLength(&head, &len);
  printf("====%d\n", len);
  printf("====%d\n", head.length);
  for(int i =1; i <=len; i++)
  {
    listGetNodeData(&head, i,&nodedata);
    printf("%d %d %d, \n", nodedata.a,nodedata.b,nodedata.c);
  }
  printf("\n");

  //listDeleteNodeData test
  listDeleteNodeData(&head, head.length,&nodedata);
  printf("delete:%d %d %d, \n", nodedata.a,nodedata.b,nodedata.c);
  listLength(&head, &len);
  printf("====%d\n", len);
  printf("====%d\n", head.length);
  for(int i =1; i <=head.length; i++)
  {
    listGetNodeData(&head, i,&nodedata);
    printf("%d %d %d, \n", nodedata.a,nodedata.b,nodedata.c);
  }
  printf("\n");

  //listInsertNodeData test
  nodedata.a = 56;
  nodedata.b = 57;
  nodedata.c = 58;
  listInsertNodeData(&head, 6,nodedata);
  listLength(&head, &len);
  printf("====%d\n", len);
  printf("====%d\n", head.length);
  for(int i =1; i <=head.length; i++)
  {
    listGetNodeData(&head, i,&nodedata);
    printf("%d %d %d, \n", nodedata.a,nodedata.b,nodedata.c);
  }

  //listCheckNodeData test
  nodedata.a = 56;
  nodedata.b = 57;
  nodedata.c = 58;
  listCheckNodeData(&head, &index, nodedata);
  listLength(&head, &len);
  printf("index: %d\n", index);
  printf("====%d\n", head.length);
  for(int i =1; i <=head.length; i++)
  {
    listGetNodeData(&head, i,&nodedata);
    printf("%d %d %d, \n", nodedata.a,nodedata.b,nodedata.c);
  }

  if(listIsEmpty(&head)){
    printf("empty!\n");
  }else{
    printf("unempty!\n");
  }
  listClear(&head);
  if(listIsEmpty(&head)){
    printf("empty!\n");
  }else{
    printf("unempty!\n");
  }
  return 0;
}
