#ifndef __MYSQL_OPTIONS_H
#define __MYSQL_OPTIONS_H

#ifdef  EXT_MYSQL_OPTIONS
#define EXT_MYSQL_OPTIONS
#else
#define EXT_MYSQL_OPTIONS extern
#endif

/* 头文件包含 */
#include <stdint.h>
#include <mysql/mysql.h>
#include "config.h"


//data_type取值
#define  DATA_TYPE_IS_CHAR                       0//char
#define  DATA_TYPE_IS_UNCHAR                     1//(unsigned)char
#define  DATA_TYPE_IS_INT                        2//int
#define  DATA_TYPE_IS_UNINT                      3//(unsigned)int
#define  DATA_TYPE_IS_FLOAT                      4//float
#define  DATA_TYPE_IS_CHAR_POINTER               5//指针char*
#define  DATA_TYPE_IS_UNCHAR_POINTER             6//指针unsigned char*
#define  DATA_TYPE_IS_INT_POINTER                7//指针int*
#define  DATA_TYPE_IS_UNINT_POINTER              8//指针unsigned int*
#define  DATA_TYPE_IS_FLOAT_POINTER              9//指针float*
#define  DATA_TYPE_IS_CHAR_ARRAY                10//char[]字符串
typedef struct{
  char*   field_name   ;//字段名称
  char*   field_type   ;//字段类型
  int32_t buffer_type  ;//数据操作时buffer_type的类型
  int32_t buffer_length;//数据操作时buffer的数据长度
  int32_t data_type    ;//定义变量的数据类型标识
  int32_t offset       ;//定义的变量相对结构体地址的偏移
}mysql_field_t;

typedef struct{
  char*          table_name;
  int32_t        filed_num;
  mysql_field_t  field[100];
}mysql_table_t;

//MYSQL字段序列
#define  MYSQL_FIELD_ITEM_0                      (1UL<<0)
#define  MYSQL_FIELD_ITEM_1                      (1UL<<1)
#define  MYSQL_FIELD_ITEM_2                      (1UL<<2)
#define  MYSQL_FIELD_ITEM_3                      (1UL<<3)
#define  MYSQL_FIELD_ITEM_4                      (1UL<<4)
#define  MYSQL_FIELD_ITEM_5                      (1UL<<5)
#define  MYSQL_FIELD_ITEM_6                      (1UL<<6)
#define  MYSQL_FIELD_ITEM_7                      (1UL<<7)
#define  MYSQL_FIELD_ITEM_8                      (1UL<<8)
#define  MYSQL_FIELD_ITEM_9                      (1UL<<9)
#define  MYSQL_FIELD_ITEM_10                     (1UL<<10)
#define  MYSQL_FIELD_ITEM_11                     (1UL<<11)
#define  MYSQL_FIELD_ITEM_12                     (1UL<<12)
#define  MYSQL_FIELD_ITEM_13                     (1UL<<13)
#define  MYSQL_FIELD_ITEM_14                     (1UL<<14)
#define  MYSQL_FIELD_ITEM_15                     (1UL<<15)
#define  MYSQL_FIELD_ITEM_16                     (1UL<<16)
#define  MYSQL_FIELD_ITEM_17                     (1UL<<17)
#define  MYSQL_FIELD_ITEM_18                     (1UL<<18)
#define  MYSQL_FIELD_ITEM_19                     (1UL<<19)
#define  MYSQL_FIELD_ITEM_20                     (1UL<<20)
#define  MYSQL_FIELD_ITEM_21                     (1UL<<21)
#define  MYSQL_FIELD_ITEM_22                     (1UL<<22)
#define  MYSQL_FIELD_ITEM_23                     (1UL<<23)
#define  MYSQL_FIELD_ITEM_24                     (1UL<<24)
#define  MYSQL_FIELD_ITEM_25                     (1UL<<25)
#define  MYSQL_FIELD_ITEM_26                     (1UL<<26)
#define  MYSQL_FIELD_ITEM_27                     (1UL<<27)
#define  MYSQL_FIELD_ITEM_28                     (1UL<<28)
#define  MYSQL_FIELD_ITEM_29                     (1UL<<29)
#define  MYSQL_FIELD_ITEM_30                     (1UL<<30)
#define  MYSQL_FIELD_ITEM_31                     (1UL<<31)

//数据库存储数据区
typedef struct{
  int32_t    int_demo;             //有符号整形
  uint32_t   uint_demo;            //无符号整形
  float      float_demo;           //浮点形
  char       string_demo[128];     //字符串
}mysql_demo_t;

/* 公有函数声明 */
EXT_MYSQL_OPTIONS  MYSQL*  mysql_connect_init(void);
EXT_MYSQL_OPTIONS  void    mysql_demo_create_table(MYSQL* mysql_fd);
EXT_MYSQL_OPTIONS  int32_t mysql_demo_get_records_num(MYSQL* mysql_fd, \
                                                      mysql_table_t mysql_table, \
                                                      uint32_t which, \
                                                      mysql_demo_t mysql_demo);
EXT_MYSQL_OPTIONS  void    mysql_demo_insert_record(MYSQL* mysql_fd, \
                                                    mysql_table_t mysql_table, \
                                                    mysql_demo_t mysql_demo);
EXT_MYSQL_OPTIONS  void    mysql_demo_delete_record(MYSQL* mysql_fd, \
                                                    mysql_table_t mysql_table, \
                                                    uint32_t which, \
                                                    mysql_demo_t mysql_demo);
EXT_MYSQL_OPTIONS  void    mysql_demo_update_record(MYSQL* mysql_fd, \
                                                    mysql_table_t mysql_table, \
                                                    uint32_t set_item, \
                                                    uint32_t which, \
                                                    mysql_demo_t mysql_demo);
EXT_MYSQL_OPTIONS  void    mysql_demo_query_record(MYSQL* mysql_fd, \
                                                   mysql_table_t mysql_table, \
                                                   uint32_t which, \
                                                   mysql_demo_t* mysql_demo);

#endif /* #ifndef __MYSQL_OPTIONS_H */
