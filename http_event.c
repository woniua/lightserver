#define EXT_HTTP_EVENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <event2/event.h>

#include "http_event.h"
#include "config.h"

void* http_event(void* arg)
{
  struct event_base*      http_base;

  http_base = http_event_arg.http_base;
  http_base = event_base_new();
  if(!http_base){
    fprintf(stderr, "Could not initialize libevent!\n");
    exit(-1);
  }

  while(1)
  {
    printf("hello http.\n");
    sleep(1);
  }
}

void http_event_thread_create(evutil_socket_t socket_fd)
{
  pthread_t http_pid;

  if(pthread_create(&http_pid, NULL, http_event, (void*)&socket_fd)){
    fprintf(stderr, "Could not create http_event_process thread!\n");
    exit(-1);
  }
  pthread_detach(http_pid);
}
