#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <event2/event.h>
#include <event2/listener.h>

#include "config.h"

void listener_cb(struct evconnlistener* listener, evutil_socket_t new_soclet_fd,\
                 struct sockaddr* saddr, int socklen, void* arg)
{
  printf("hello junliang.\n");
}

int main(int argc, char const *argv[])
{
  struct event_base*      listen_base;
  struct evconnlistener*  listener;
  struct sockaddr_in      sock_in;

  listen_base = event_base_new();
  if(!listen_base){
    fprintf(stderr, "Couldn't create an event_base:exiting\n");
    return -1;
  }

  memset(&sock_in, 0, sizeof(sock_in));
  sock_in.sin_family = AF_INET;
  sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_in.sin_port = htons(HTTP_PORT);
  listener = evconnlistener_new_bind(listen_base, listener_cb, (char*)"hello http",
                                    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
                                    (struct sockaddr*)&sock_in, sizeof(sock_in) );
  event_base_dispatch(listen_base);

  evconnlistener_free(listener);
  event_base_free(listen_base);
  printf("done.\n");
  return 0;
}
