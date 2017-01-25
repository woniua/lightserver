#define EXT_PORTABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <event2/event.h>
#include <event2/util.h>

#include "portable.h"
#include "config.h"

void portable_tcp_dev_ack_handle(const uint8_t* read_buffer, \
                                 const uint32_t read_length, \
                                 uint8_t*  write_buffer, \
                                 uint32_t* write_length)
{
  char send_data[] = "hello everyone!!!";
  memcpy(write_buffer, send_data, sizeof(send_data));
  *write_length = sizeof(send_data);
}

void  portable_tcp_monitor_ack_handle(const uint8_t* read_buffer, \
                                      const uint32_t read_length, \
                                      uint8_t*  write_buffer, \
                                      uint32_t* write_length)
{

}
