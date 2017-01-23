#include <stdio.h>
#include <unistd.h>

#include "../../../aes_options.h"

void debug_print_in_hex(uint8_t* buf, uint32_t len)
{
  uint32_t i;
  int8_t   print_buf[3];

  for(i = 0; i < len; i++){
    sprintf(print_buf, "%02x", *(buf + i));
    printf("%s ", print_buf);
  }
  printf("\n\n");
}

int main(int argc, char const *argv[])
{
  //uint8_t   content[]={0x11,0x12,0x14,0x16};
  uint8_t   content[]   = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12};
  uint8_t   buffer1[106] = {0};
  uint8_t   buffer2[106] = {0};
  uint32_t  len;
  printf("input:\n");
  debug_print_in_hex(content, sizeof(content)/sizeof(content[0]));
  //printf("%s\n\n", content);

  len = encrypt(content, buffer1, sizeof(content)/sizeof(content[0]));
  printf("encrypt:\n");
  debug_print_in_hex(buffer1, len);

  decrypt(buffer1, buffer2, len);
  printf("decrypt:\n");
  debug_print_in_hex(buffer2, len);
  //printf("%s\n", buffer2);

  return 0;
}
