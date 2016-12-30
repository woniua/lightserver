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

/* 公有函数声明 */
EXT_MYSQL_OPTIONS  MYSQL* mysql_connect_init(void);
//EXT_MYSQL_OPTIONS  int    mysql_create_table(MYSQL* mysql_fd, char* table, ...);

#endif /* #ifndef __MYSQL_OPTIONS_H */
