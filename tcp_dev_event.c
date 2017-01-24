#define EXT_TCP_DEV_EVENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <event2/event.h>
#include <event2/util.h>

#include "tcp_dev_event.h"
#include "log_options.h"
#include "list.h"
#include "config.h"

void bev_tcp_dev_read_cb(struct bufferevent* bev, void* ctx)
{
  char data[200]= {0};
  char senddata[100] ="Hello I'm tcp-dev!!!\n";
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

void bev_tcp_dev_event_cb(struct bufferevent* bev, short events, void* ctx)
{
  nodedata_t  nodedata_tmp = {0};
  char        tmp[512]     = {0};
  int32_t     index        = 0;

  if(events &  BEV_EVENT_READING){//读数据过程中发生错误
    if(events &  BEV_EVENT_TIMEOUT){
      log_handle(LOG_TYPE_DISCONNECT, CFG_TCP_DEV_PORT, bufferevent_getfd(bev), "<reading> time out.", 0);
    }
    else if(events &  BEV_EVENT_EOF){
      log_handle(LOG_TYPE_DISCONNECT, CFG_TCP_DEV_PORT, bufferevent_getfd(bev), "<reading> reached end-of-file.", 0);
    }
    else if(events &  BEV_EVENT_ERROR){
      memset(tmp, 0, sizeof(tmp)/sizeof(tmp[0]));
      sprintf(tmp,"<writing> %s", strerror(EVUTIL_SOCKET_ERROR()));
      log_handle(LOG_TYPE_DISCONNECT, CFG_TCP_DEV_PORT, bufferevent_getfd(bev), tmp, 0);
    }
    else{
      fprintf(stderr, "unkown reading error type on function '%s'\n", __FUNCTION__);
      return;
    }
  }
  else if(events &  BEV_EVENT_WRITING){//写数据过程中发生错误
    if(events &  BEV_EVENT_TIMEOUT){
      log_handle(LOG_TYPE_DISCONNECT, CFG_TCP_DEV_PORT, bufferevent_getfd(bev), "<writing> time out.", 0);
    }
    else if(events &  BEV_EVENT_EOF){
      log_handle(LOG_TYPE_DISCONNECT, CFG_TCP_DEV_PORT, bufferevent_getfd(bev), "<writing> reached end-of-file.", 0);
    }
    else if(events &  BEV_EVENT_ERROR){
      memset(tmp, 0, sizeof(tmp)/sizeof(tmp[0]));
      sprintf(tmp,"<writing> %s", strerror(EVUTIL_SOCKET_ERROR()));
      log_handle(LOG_TYPE_DISCONNECT, CFG_TCP_DEV_PORT, bufferevent_getfd(bev), tmp, 0);
    }
    else{
      fprintf(stderr, "unkown writing error type on function '%s'\n", __FUNCTION__);
      return;
    }
  }
  else{
    fprintf(stderr, "unkown error type on function '%s'\n", __FUNCTION__);
    return;
  }

  //释放bufferevent资源，释放链表资源,关闭网络连接描述符
  index = list_check_nodedata(&tcp_dev_head, bufferevent_getfd(bev), &nodedata_tmp);
  if(index != -1){
    list_delete_nodedata(&tcp_dev_head, index, &nodedata_tmp);
  }
  bufferevent_free(bev);
}

void* tcp_dev_event_thread_process(void* arg)
{
  tcp_dev_event_arg.eventbase = event_base_new();
  if(!tcp_dev_event_arg.eventbase){
    fprintf(stderr, "could not initialize libevent!\n");
    exit(-1);
  }

  while(1){
    event_base_dispatch(tcp_dev_event_arg.eventbase);
    sleep(1);
  }
  event_base_free(tcp_dev_event_arg.eventbase);
}

void tcp_dev_event_thread_create(evutil_socket_t socket_fd)
{
  pthread_t tcp_read_pid;

  if(pthread_create(&tcp_read_pid, NULL, tcp_dev_event_thread_process, (void*)&socket_fd)){
    fprintf(stderr, "could not create http_event_process thread!\n");
    exit(-1);
  }
  pthread_detach(tcp_read_pid);
}
