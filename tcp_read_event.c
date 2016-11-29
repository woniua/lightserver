#define EXT_TCP_READ_EVENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <event2/event.h>

#include "tcp_read_event.h"
#include "config.h"

void cb(evutil_socket_t a, short b, void * c)
{
  printf("read wwwwwwwwwwwwwwwwwwwww\n");
}

void* tcp_read_event(void* arg)
{
  struct event*  timeout_event;
  struct event*  timeout_event1;
  struct timeval timeout = {20,0};
  //struct timeval timeout1 = {5,0};
  tcp_read_event_arg.tcp_read_base = event_base_new();
  if(!tcp_read_event_arg.tcp_read_base){
    fprintf(stderr, "Could not initialize libevent!\n");
    exit(-1);
  }
  timeout_event = evtimer_new(tcp_read_event_arg.tcp_read_base, cb, NULL);
  evtimer_add(timeout_event, &timeout);
  timeout_event1 = evtimer_new(tcp_read_event_arg.tcp_read_base, cb, NULL);
  //evtimer_add(timeout_event1, &timeout1);
  event_base_dispatch(tcp_read_event_arg.tcp_read_base);
  printf("qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq\n");
  event_base_free(tcp_read_event_arg.tcp_read_base);
}

void tcp_read_event_thread_create(evutil_socket_t socket_fd)
{
  pthread_t tcp_read_pid;

  if(pthread_create(&tcp_read_pid, NULL, tcp_read_event, (void*)&socket_fd)){
    fprintf(stderr, "Could not create http_event_process thread!\n");
    exit(-1);
  }
  pthread_detach(tcp_read_pid);
}
