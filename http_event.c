#define EXT_HTTP_EVENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <event2/event.h>

#include "http_event.h"
#include "config.h"

void* http_event_thread_process(void* arg)
{
  struct event_base*      eventbase;

  eventbase = http_event_arg.eventbase;
  eventbase = event_base_new();
  if(!eventbase){
    fprintf(stderr, "Could not initialize libevent!\n");
    exit(-1);
  }

  while(1){
    printf("hello http.\n");
    //event_base_dispatch(http_event_arg.eventbase);
    sleep(1);
  }
  event_base_free(http_event_arg.eventbase);
}

void http_event_thread_create(evutil_socket_t socket_fd)
{
  pthread_t http_pid;

  if(pthread_create(&http_pid, NULL, http_event_thread_process, (void*)&socket_fd)){
    fprintf(stderr, "Could not create http_event_process thread!\n");
    exit(-1);
  }
  pthread_detach(http_pid);
}
