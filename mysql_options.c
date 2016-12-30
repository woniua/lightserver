#define EXT_MYSQL_OPTIONS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "mysql_options.h"
#include "config.h"

static void mysql_stmt_error_print(MYSQL_STMT* stmt, int32_t status)
{
  char err_buf[1024] = {0};
  if(status)
  {
    fprintf(stderr, "Error: %s (errno: %d)\n", mysql_stmt_error(stmt), mysql_stmt_errno(stmt));
    sprintf(err_buf, "Error: %s (errno: %d)\n", mysql_stmt_error(stmt), mysql_stmt_errno(stmt));
  }
}

static int mysql_create_table(MYSQL* mysql_fd, char* table, ...)
{
  char     create_table[1024] ={0};
  char     arg_buf[788]       ={0};
  char*    argp = NULL;
  va_list  args;
  int      rtn = 0;

  va_start(args, table);
  while(1)
  {
    argp = va_arg(args, char*);
    if(argp != NULL){
      strcat(&arg_buf[strlen(arg_buf)], argp);
      strcat(&arg_buf[strlen(arg_buf)], ", ");
    }
    else{
      arg_buf[strlen(arg_buf) -2] = 0;
      break;
    }
  }
  va_end(args);
  sprintf(create_table,"create table IF NOT EXISTS %s (%s)", table, arg_buf);
  if(0 == mysql_ping(mysql_fd)){//连接处于活动状态
    if( 0 == mysql_real_query(mysql_fd,create_table,strlen(create_table)) ){
      printf("create table %s success!\n", table);
    }
    else{
      rtn = -1;
      printf("create table %s failed!\n", table);
    }
  }else{
    rtn = -1;
  }
  return rtn;
}

MYSQL* mysql_connect_init(void)
{
  MYSQL*  mysql_fd;
  my_bool reconnect = 1;
  int     i;

  mysql_fd = mysql_init(NULL);
  //使能自动重连
  mysql_options(mysql_fd, MYSQL_OPT_RECONNECT, &reconnect);
  if( NULL != mysql_real_connect(mysql_fd, \
                                 CFG_MYSQL_HOST, \
                                 CFG_MYSQL_USER, \
                                 CFG_MYSQL_PASSWORD, \
                                 CFG_MYSQL_DATABASE, \
                                 0,0,0) )
  {
    printf("mysql connection success!\n");
  }
  else
  {
    printf("mysql connection error: %s\n", mysql_error(mysql_fd) );
    return NULL;
  }
  return mysql_fd;
}

int mysql_11_create_table(MYSQL* mysql_fd)
{
  return mysql_create_table(mysql_fd, "usrpasswd", \
                                      "usrname char(8)", \
                                      "passwd char(8)", \
                                      "register_date char(13)", \
                                      "login_date char(13)",\
                                      "company_name char(32)", NULL);
}
