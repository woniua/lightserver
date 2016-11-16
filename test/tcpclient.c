#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void help_disp(int expectArgc, int actualArgc)
{
  char* helpStr = "Useage: ./[App] [ClientNum] [DirIP] [DirPort] [Msg].\n"
                  "[App      ] : this program.\n"
                  "[ClientNum] : number that you want create.\n"
                  "[DirIP    ] : direction server IP address.\n"
                  "[DirPort  ] : direction server IP port.\n"
                  "[Msg      ] : message that you want send to server.\n";

  if(actualArgc < expectArgc){
    printf("Less argument than expected.\n");
  }
  else if(actualArgc > expectArgc){
    printf("More argument than expected.\n");
  }
  printf("%s\n", helpStr);
}

int main(int argc, char const *argv[])
{
  if(argc != 4){
    help_disp(4, argc);
  }

  
  return 0;
}
