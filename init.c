#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <event2/event.h>
#include <event2/listener.h>

#include <pthread.h>

#include "list.h"
#include "http_event_process.h"
#include "tcp_event_process.h"
#include "config.h"

void listener_http_cb(struct evconnlistener* listener, evutil_socket_t new_soclet_fd,\
                 struct sockaddr* saddr, int socklen, void* arg)
{
  printf("hello http junliang.\n");
}

void listener_tcp_cb(struct evconnlistener* listener, evutil_socket_t new_soclet_fd,\
                 struct sockaddr* saddr, int socklen, void* arg)
{
  printf("hello tcp junliang.\n");
}

int main(int argc, char const *argv[])
{
  struct event_base*      listen_base;
  struct evconnlistener*  listener;
  struct sockaddr_in      sock_in;

  pthread_t http_pid;
  pthread_t tcp_pid;

  ///////////////////////////TEST////////////////////////////
  uint32_t len = 0;
  listCreate(&list, 100);
  listLength(list, &len);
  printf("====%d\n", len);
  ///////////////////////////TEST////////////////////////////

  //打印lightserver版本信息
  printf("Lightserver: %d.%d.%d\n",MAJOR_VERSION,MINOR_VERSION,REVISION_VERSION);

  //打印libevent版本信息
  printf("Used libevent version: %s\n", event_get_version());

  //创建http-event-dispath线程
  if(pthread_create(&http_pid, NULL, http_event_process, (void*)&http_event_arg)){
    fprintf(stderr, "Could not create http_event_process thread!\n");
    exit(-1);
  }

  //创建tcp-event-dispath线程
  if(pthread_create(&tcp_pid, NULL, tcp_event_process, (void*)&tcp_event_arg)){
    fprintf(stderr, "Could not create tcp_event_process thread!\n");
    exit(-1);
  }

  listen_base = event_base_new();
  if(!listen_base){
    fprintf(stderr, "Could not initialize libevent!\n");
    exit(-1);
  }

  memset(&sock_in, 0, sizeof(sock_in));
  sock_in.sin_family      = AF_INET;
  sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_in.sin_port        = htons(HTTP_PORT);
  listener = evconnlistener_new_bind(listen_base, listener_http_cb, (char*)"hello http",
                                     LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
                                    (struct sockaddr*)&sock_in, sizeof(sock_in) );
  if(!listener){
    fprintf(stderr, "Could not create a listener!\n");
    exit(-1);
  }

  memset(&sock_in, 0, sizeof(sock_in));
  sock_in.sin_family      = AF_INET;
  sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_in.sin_port        = htons(TCP_PORT);
  listener = evconnlistener_new_bind(listen_base, listener_tcp_cb, (char*)"hello tcp",
                                     LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
                                    (struct sockaddr*)&sock_in, sizeof(sock_in) );
  if(!listener){
    fprintf(stderr, "Could not create a listener!\n");
    exit(-1);
  }

  event_base_dispatch(listen_base);

  evconnlistener_free(listener);
  event_base_free(listen_base);
  printf("done.\n");
  return 0;
}
