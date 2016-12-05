#define EXT_LIST

#include <stdio.h>
#include <unistd.h>

#include "../../list.h"

int main(int argc, char const *argv[])
{
  uint32_t     len = 0;
  uint32_t     index =0;
  nodedata_t   nodedata={0};
  list_create(&head, 0);
  list_length(&head, &len);
  printf("====%d\n", len);
  printf("====%d\n", head.length);
  for(int i =1; i <=len; i++)
  {
    list_get_nodedata(&head, i,&nodedata);
    printf("%d %d %d, \n", nodedata.a,nodedata.b,nodedata.c);
  }
  printf("\n");

  //list_delete_nodedata test
  list_delete_nodedata(&head, head.length,&nodedata);
  printf("delete:%d %d %d, \n", nodedata.a,nodedata.b,nodedata.c);
  list_length(&head, &len);
  printf("====%d\n", len);
  printf("====%d\n", head.length);
  for(int i =1; i <=head.length; i++)
  {
    list_get_nodedata(&head, i,&nodedata);
    printf("%d %d %d, \n", nodedata.a,nodedata.b,nodedata.c);
  }
  printf("\n");

  //list_insert_nodedata test
  nodedata.a = 56;
  nodedata.b = 57;
  nodedata.c = 58;
  list_insert_nodedata(&head, 1,nodedata);
  list_length(&head, &len);
  printf("====%d\n", len);
  printf("====%d\n", head.length);
  for(int i =1; i <=head.length; i++)
  {
    list_get_nodedata(&head, i,&nodedata);
    printf("%d %d %d, \n", nodedata.a,nodedata.b,nodedata.c);
  }

  //list_check_nodedata test
  nodedata.a = 56;
  nodedata.b = 57;
  nodedata.c = 58;
  list_check_nodedata(&head, &index, nodedata);
  list_length(&head, &len);
  printf("index: %d\n", index);
  printf("====%d\n", head.length);
  for(int i =1; i <=head.length; i++)
  {
    list_get_nodedata(&head, i,&nodedata);
    printf("%d %d %d, \n", nodedata.a,nodedata.b,nodedata.c);
  }

  if(list_is_empty(&head)){
    printf("empty!\n");
  }else{
    printf("unempty!\n");
  }
  list_clear(&head);
  if(list_is_empty(&head)){
    printf("empty!\n");
  }else{
    printf("unempty!\n");
  }
  return 0;
}
