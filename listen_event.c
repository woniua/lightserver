#define EXT_LISTEN_EVENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <event2/event.h>
#include <event2/listener.h>

#include "listen_event.h"
#include "config.h"

void listener_http_cb(struct evconnlistener* listener, evutil_socket_t new_socket_fd,\
                      struct sockaddr* saddr, int socklen, void* arg)
{
  evutil_make_socket_nonblocking(new_socket_fd);//设置socket为非阻塞
  printf("hello http junliang.\n");
}

void listener_tcp_cb(struct evconnlistener* listener, evutil_socket_t new_socket_fd,\
                     struct sockaddr* saddr, int socklen, void* arg)
{
  evutil_make_socket_nonblocking(new_socket_fd);//设置socket为非阻塞
  printf("hello tcp junliang.\n");
}

void listen_event_handle(void)
{
  struct event_base*      listen_base;
  struct evconnlistener*  listener;
  struct sockaddr_in      sock_in;

  listen_base = listen_event_arg.listen_base;
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
}
