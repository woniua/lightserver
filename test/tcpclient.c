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

void* client_thread(void* arg)
{
  struct hostend*    he;
  struct sockaddr_in server;

  if((he = gethostname(argv[2]))==NULL){
    fprintf(stderr, "gethostname() error!\n");
    exit(-1);
  }
  
}

void client_thread_create(void)
{
  pthread_t thread_pid;

  if(pthread_create(&thread_pid, NULL, client_thread, (void*)NULL)){
    fprintf(stderr, "Could not create client thread!\n");
    exit(-1);
  }
  pthread_detach(thread_pid);
}

int main(int argc, char const *argv[])
{
  if(argc != 4){
    help_disp(4, argc);
  }

//创建ClientNum个线程
for(int i = 0; i < atoi(argv[1]); i++){
  client_thread_create();
}
  return 0;
}
