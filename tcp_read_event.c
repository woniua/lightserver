#define EXT_TCP_READ_EVENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <event2/event.h>

#include "tcp_read_event.h"
#include "config.h"

void* tcp_read_event(void* arg)
{
  struct event_base*      tcp_read_base;

  tcp_read_base = tcp_read_event_arg.tcp_read_base;
  tcp_read_base = event_base_new();
  if(!tcp_read_base){
    fprintf(stderr, "Could not initialize libevent!\n");
    exit(-1);
  }

  while(1)
  {
    printf("hello tcp read.\n");
    sleep(1);
  }
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
