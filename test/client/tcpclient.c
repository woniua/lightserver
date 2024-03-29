#define EXT_TCPCLIENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> //for struct hostend
#include <arpa/inet.h>
#include <math.h>
#include <time.h>

#include "tcpclient.h"

void help_disp(int expectArgc, int actualArgc)
{
  char* helpString = "Useage: ./[App] [ClientNum] [DirIP] [DirPort] [Msg].\n"
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
  printf("%s\n", helpString);
}

void* client_thread(void* arg)
{
  struct hostent*    he;
  struct sockaddr_in server;
  int                connect_fd;
  char               recvbuf[1024] = {0};
  tcpclient_arg_t* tcpclient_arg = (tcpclient_arg_t*)arg;

  //向SERVER发起连接
  if((he = gethostbyname(tcpclient_arg->serverAddr))==NULL){
    fprintf(stderr, "gethostbyname() error!\n");
    pthread_exit(NULL);
  }
  if((connect_fd = socket(AF_INET, SOCK_STREAM, 0))==-1){
    fprintf(stderr, "socket() error!\n");
    pthread_exit(NULL);
  }

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port   = tcpclient_arg->serverPort;
  server.sin_addr   = *((struct in_addr*)he->h_addr);

  if(connect(connect_fd, (struct sockaddr*)&server, sizeof(struct sockaddr))==-1){
    fprintf(stderr, "connect() error!\n");
    pthread_exit(NULL);
  }

  //设置发送超时时间
  struct timeval send_timeout = {SOCKET_SEND_TIMEOUT_S, 0};
  setsockopt(connect_fd, SOL_SOCKET, SO_SNDTIMEO, &send_timeout, sizeof(send_timeout));
  //设置接收超时时间
  struct timeval recv_timeout = {SOCKET_RECV_TIMEOUT_S, 0};
  setsockopt(connect_fd, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof(recv_timeout));
  while(1)
  {
    if(-1 == send(connect_fd, tcpclient_arg->msg, strlen(tcpclient_arg->msg), 0)){
      close(connect_fd);
      pthread_exit(NULL);
    }
    printf("connect-fd %d send:%s\n", connect_fd, tcpclient_arg->msg);
    if(recv(connect_fd, recvbuf, sizeof(recvbuf), 0) > 0){
      printf("connect-fd %d recv:%s\n",connect_fd, recvbuf);
      memset(recvbuf, 0, sizeof(recvbuf));
      recvcount++;
    }
    else{
      printf("\n");
      printf("client number     :%d.\n",  tcpclient_arg->clientNum);
      printf("recv timeout value:%ds.\n", SOCKET_RECV_TIMEOUT_S);
      printf("recv total count  :%d.\n", recvcount);
      exit(-1);
    }

    sleep(1);
  }
}

void client_thread_create(tcpclient_arg_t* tcpclient_arg)
{
  pthread_t thread_pid;

  if(pthread_create(&thread_pid, NULL, client_thread, (void*)tcpclient_arg)){
    fprintf(stderr, "Could not create client thread!\n");
    exit(-1);
  }
  pthread_detach(thread_pid);
}

int main(int argc, char const *argv[])
{
  if(argc != 5){
    help_disp(5, argc);
    exit(-1);
  }

  //传入参数赋值给结构体
  tcpclient_arg.clientNum  = atoi(argv[1]);
  tcpclient_arg.serverAddr = (char*)argv[2];
  tcpclient_arg.serverPort = htons(atoi(argv[3]));
  tcpclient_arg.msg        = (char*)argv[4];

  //创建ClientNum个线程
  for(int i = 0; i < atoi(argv[1]); i++){
    client_thread_create(&tcpclient_arg);
  }
  while(1);
  return 0;
}
