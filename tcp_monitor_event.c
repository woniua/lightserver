#define EXT_TCP_MONITOR_EVENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <event2/event.h>
#include <event2/util.h>

#include "tcp_monitor_event.h"
#include "config.h"

void bev_tcp_monitor_read_cb(struct bufferevent* bev, void* ctx)
{
  char data[200]= {0};
  char senddata[100] ="Hello I'm tcp-monitor!!!\n";
  uint32_t length;
  struct evbuffer* tmp = evbuffer_new();
  if(!tmp) return;

  length = bufferevent_read_buffer(bev, tmp);
  evbuffer_remove(tmp, data, sizeof(data));
  printf("read cd:%d-%s\n",length, data);

  evbuffer_add(tmp, senddata, strlen(senddata));
  evbuffer_write(tmp, bufferevent_getfd(bev));
  printf("send : %s", senddata);
  evbuffer_free(tmp);
}

void bev_tcp_monitor_event_cb(struct bufferevent* bev, short events, void* ctx)
{
  printf("event cb\n");
  //bufferevent_free(bev);
}

void* tcp_monitor_event_thread_process(void* arg)
{
  evutil_socket_t socket_fd = *(evutil_socket_t*)arg;
  tcp_monitor_event_arg.tcp_eventbase = event_base_new();
  if(!tcp_monitor_event_arg.tcp_eventbase){
    fprintf(stderr, "Could not initialize libevent!\n");
    exit(-1);
  }

  while(1){
    event_base_dispatch(tcp_monitor_event_arg.tcp_eventbase);
    sleep(1);
  }
  event_base_free(tcp_monitor_event_arg.tcp_eventbase);
}

void tcp_monitor_event_thread_create(evutil_socket_t socket_fd)
{
  pthread_t tcp_read_pid;

  if(pthread_create(&tcp_read_pid, NULL, tcp_monitor_event_thread_process, (void*)&socket_fd)){
    fprintf(stderr, "Could not create http_event_process thread!\n");
    exit(-1);
  }
  pthread_detach(tcp_read_pid);
}
