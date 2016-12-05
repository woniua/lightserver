#define EXT_TCP_EVENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <event2/event.h>
#include <event2/util.h>

#include "tcp_event.h"
#include "config.h"

void bev_read_cb(struct bufferevent *bev, void *ctx)
{
  printf("read cd\n");
}

void bev_write_cb(struct bufferevent *bev, void *ctx)
{
  printf("write cd\n");
}

void bev_event_cb(struct bufferevent *bev, short events, void *ctx)
{
  printf("event cb\n");
}

void* tcp_event_thread_process(void* arg)
{
  evutil_socket_t socket_fd = *(evutil_socket_t*)arg;
  tcp_event_arg.tcp_eventbase = event_base_new();
  if(!tcp_event_arg.tcp_eventbase){
    fprintf(stderr, "Could not initialize libevent!\n");
    exit(-1);
  }

  while(1)
  {
    event_base_dispatch(tcp_event_arg.tcp_eventbase);
    sleep(1);
  }
  event_base_free(tcp_event_arg.tcp_eventbase);
}

void tcp_event_thread_create(evutil_socket_t socket_fd)
{
  pthread_t tcp_read_pid;

  if(pthread_create(&tcp_read_pid, NULL, tcp_event_thread_process, (void*)&socket_fd)){
    fprintf(stderr, "Could not create http_event_process thread!\n");
    exit(-1);
  }
  pthread_detach(tcp_read_pid);
}
