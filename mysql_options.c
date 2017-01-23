#define EXT_MYSQL_OPTIONS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "mysql_options.h"
#include "config.h"

/*****
* 根据mysql_demo_t结构体生成mysql_table数据
*/
static  mysql_table_t mysql_table;

static void mysql_table_assign(mysql_table_t* mysql_table)
{
  mysql_demo_t demo_tmp;
  int32_t      i = 0;

  mysql_table ->table_name = "demo_table";
  mysql_table ->filed_num  = 4;

  mysql_table ->field[i].field_name    = "int_demo";
  mysql_table ->field[i].field_type    = "int(10)";
  mysql_table ->field[i].buffer_type   = MYSQL_TYPE_LONG;
  mysql_table ->field[i].buffer_length = sizeof(int);
  mysql_table ->field[i].data_type     = DATA_TYPE_IS_INT;
  mysql_table ->field[i].offset        = (uint64_t)(&(demo_tmp.int_demo))-(uint64_t)(&demo_tmp);
  printf("offset = %d.\n", mysql_table ->field[i].offset);
  i++;
  mysql_table ->field[i].field_name    = "uint_demo";
  mysql_table ->field[i].field_type    = "int(10) unsigned";
  mysql_table ->field[i].buffer_type   = MYSQL_TYPE_LONG;
  mysql_table ->field[i].buffer_length = sizeof(int);
  mysql_table ->field[i].data_type     = DATA_TYPE_IS_UNINT;
  mysql_table ->field[i].offset        = (uint64_t)(&(demo_tmp.uint_demo))-(uint64_t)(&demo_tmp);
  printf("offset = %d.\n", mysql_table ->field[i].offset);
  i++;
  mysql_table ->field[i].field_name    = "float_demo";
  mysql_table ->field[i].field_type    = "float(4,3)";
  mysql_table ->field[i].buffer_type   = MYSQL_TYPE_FLOAT;
  mysql_table ->field[i].buffer_length = sizeof(float);
  mysql_table ->field[i].data_type     = DATA_TYPE_IS_FLOAT;
  mysql_table ->field[i].offset        = (uint64_t)(&(demo_tmp.float_demo))-(uint64_t)(&demo_tmp);
  printf("offset = %d.\n", mysql_table ->field[i].offset);
  i++;
  mysql_table ->field[i].field_name    = "string_demo";
  mysql_table ->field[i].field_type    = "varchar(128)";
  mysql_table ->field[i].buffer_type   = MYSQL_TYPE_STRING;
  mysql_table ->field[i].buffer_length = sizeof(demo_tmp.string_demo)/sizeof(demo_tmp.string_demo[0]);
  mysql_table ->field[i].data_type     = DATA_TYPE_IS_CHAR_ARRAY;
  mysql_table ->field[i].offset        =(uint64_t)(&(demo_tmp.string_demo[0]))-(uint64_t)(&demo_tmp);
  printf("offset = %d.\n", mysql_table ->field[i].offset);
  i++;
  mysql_table ->field[i++].field_name = NULL;
  mysql_table ->field[i++].field_type = NULL;
}

static void mysql_stmt_error_print(MYSQL_STMT* stmt, int32_t status)
{
  char err_buf[1024] = {0};
  if(status){
    fprintf(stderr , "Error: %s (errno: %d)\n", mysql_stmt_error(stmt), mysql_stmt_errno(stmt));
    sprintf(err_buf, "Error: %s (errno: %d)\n", mysql_stmt_error(stmt), mysql_stmt_errno(stmt));
  }
}

static int mysql_create_table(MYSQL* mysql_fd, char* table, ...)
{
  char     create_table[2048] = {0};
  char     arg_buf[1788]      = {0};
  char*    argp               = NULL;
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
  }
  else{
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
                                 0,0,0) ){
    printf("mysql connection success!\n");
  }
  else{
    printf("mysql connection error: %s\n", mysql_error(mysql_fd) );
    return NULL;
  }
  return mysql_fd;
}

void mysql_demo_create_table(MYSQL* mysql_fd)
{
  uint32_t i               = 0;
  char     field_tmp[2048] = {"recordDate TIMESTAMP, "};//自动记录record时间

  mysql_table_assign(&mysql_table);//test
  while(mysql_table.field[i].field_name != NULL){
    sprintf(&field_tmp[strlen(field_tmp)], "%s %s, ", \
            mysql_table.field[i].field_name, \
            mysql_table.field[i].field_type);
    i++;
  }
  field_tmp[strlen(field_tmp) - 2] = 0;
  mysql_create_table(mysql_fd, mysql_table.table_name, field_tmp, NULL);
}

int32_t mysql_demo_get_records_num(MYSQL* mysql_fd, mysql_table_t mysql_table, uint32_t which, mysql_demo_t mysql_demo)
{
  MYSQL_STMT*  st_handle;
  MYSQL_RES*   result;
  int32_t      count;
  int32_t      i = 0;
  char         cmd_tmp[1024] ={0};

  if(0 != mysql_ping(mysql_fd)){return -1;}
  sprintf(&cmd_tmp[strlen(cmd_tmp)], "delete from %s where ", \
                                     mysql_table.table_name);
  for(i = 0; i < 32; i++){
    if((which & (1<<i)) != 0){
      if(mysql_table.field[i].data_type == DATA_TYPE_IS_CHAR){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ",  \
                mysql_table.field[i].field_name, \
                *(char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_UNCHAR){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ",  \
                mysql_table.field[i].field_name, \
                *(unsigned char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_INT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ", \
                mysql_table.field[i].field_name, \
                *(int32_t*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_UNINT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%u and ", \
                mysql_table.field[i].field_name, \
                *(uint32_t*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_FLOAT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%f and ", \
                mysql_table.field[i].field_name, \
                *(float*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_CHAR_ARRAY){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s ='%s' and ", \
                mysql_table.field[i].field_name, \
                (char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else{
        fprintf(stderr, "unkown data_type on function '%s'\n", __FUNCTION__);
        return -1;
      }
    }
  }
  cmd_tmp[strlen(cmd_tmp)-4] = 0;
  if(0 == mysql_real_query(mysql_fd, cmd_tmp, strlen(cmd_tmp))){
    result = mysql_store_result(mysql_fd);
    count  = mysql_num_rows(result);
    mysql_free_result(result);
  }
  else{
    fprintf(stderr, " mysql get records num failed on function '%s'\n", __FUNCTION__);
    return -1;
  }
  return count;
}

void mysql_demo_insert_record(MYSQL* mysql_fd, mysql_table_t mysql_table, mysql_demo_t mysql_demo)
{
  MYSQL_STMT*  st_handle;
  MYSQL_BIND   para[mysql_table.filed_num];
  int32_t      i = 0;
  int32_t      status;
  char         cmd_tmp[1024] ={0};

	if(0 != mysql_ping(mysql_fd)){return;}
	memset(para,0,sizeof(para));
  sprintf(&cmd_tmp[strlen(cmd_tmp)], "insert into %s (", mysql_table.table_name);
  for(i = 0; mysql_table.field[i].field_name != NULL; i++){
    sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s, ", mysql_table.field[i].field_name);
  }
  cmd_tmp[strlen(cmd_tmp)-2] = 0;
  sprintf(&cmd_tmp[strlen(cmd_tmp)], ")values(");
  for(i = 0; i < mysql_table.filed_num; i++){
    sprintf(&cmd_tmp[strlen(cmd_tmp)], "?,");
  }
  cmd_tmp[strlen(cmd_tmp)-1] = ')';
  //printf("%s\n", cmd_tmp);
  st_handle = mysql_stmt_init(mysql_fd);
	if (!st_handle){
    fprintf(stderr, " mysql initialize statement failed on function '%s'\n", __FUNCTION__);
    return;
	}
	status = mysql_stmt_prepare(st_handle, cmd_tmp, strlen(cmd_tmp));
	mysql_stmt_error_print(st_handle, status);
  for(i = 0; i < mysql_table.filed_num; i++){
    para[i].buffer_type   = mysql_table.field[i].buffer_type;
    para[i].buffer 		    = (void*)(&mysql_demo + mysql_table.field[i].offset);
    para[i].buffer_length = mysql_table.field[i].buffer_length;
  }
  status = mysql_stmt_bind_param(st_handle, para);
	mysql_stmt_error_print(st_handle, status);
	status = mysql_stmt_execute(st_handle);
	mysql_stmt_error_print(st_handle, status);
	status = mysql_stmt_close(st_handle);
	mysql_stmt_error_print(st_handle, status);
}

void mysql_demo_delete_record(MYSQL* mysql_fd, mysql_table_t mysql_table, \
                              uint32_t which, mysql_demo_t mysql_demo)
{
  MYSQL_STMT*  st_handle;
  int32_t      i = 0;
  char         cmd_tmp[1024] ={0};

  if(0 != mysql_ping(mysql_fd)){return;}
  sprintf(&cmd_tmp[strlen(cmd_tmp)], "delete from %s where ", \
                                     mysql_table.table_name);
  for(i = 0; i < 32; i++){
    if((which & (1<<i)) != 0){
      if(mysql_table.field[i].data_type == DATA_TYPE_IS_CHAR){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ",  \
                mysql_table.field[i].field_name, \
                *(char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_UNCHAR){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ",  \
                mysql_table.field[i].field_name, \
                *(unsigned char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_INT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ", \
                mysql_table.field[i].field_name, \
                *(int32_t*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_UNINT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%u and ", \
                mysql_table.field[i].field_name, \
                *(uint32_t*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_FLOAT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%f and ", \
                mysql_table.field[i].field_name, \
                *(float*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_CHAR_ARRAY){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s ='%s' and ", \
                mysql_table.field[i].field_name, \
                (char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else{
        fprintf(stderr, "unkown data_type on function '%s'\n", __FUNCTION__);
        return;
      }
    }
  }
  cmd_tmp[strlen(cmd_tmp)-4] = 0;
  if(0 != mysql_real_query(mysql_fd, cmd_tmp, strlen(cmd_tmp))){
		fprintf(stderr, " mysql delete record failed on function '%s'\n", __FUNCTION__);
	}
}

void mysql_demo_update_record(MYSQL* mysql_fd, mysql_table_t mysql_table, \
                              uint32_t set_item, uint32_t which, mysql_demo_t mysql_demo)
{
  MYSQL_STMT*  st_handle;
  int32_t      i = 0;
  char         cmd_tmp[1024] ={0};

  if(0 != mysql_ping(mysql_fd)){return;}
  sprintf(&cmd_tmp[strlen(cmd_tmp)], "update %s set ", \
                                     mysql_table.table_name);
  for(i = 0; i < 32; i++){
    if((set_item & (1<<i)) != 0){
      if(mysql_table.field[i].data_type == DATA_TYPE_IS_CHAR){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ",  \
                mysql_table.field[i].field_name, \
                *(char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_UNCHAR){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ",  \
                mysql_table.field[i].field_name, \
                *(unsigned char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_INT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ", \
                mysql_table.field[i].field_name, \
                *(int32_t*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_UNINT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%u and ", \
                mysql_table.field[i].field_name, \
                *(uint32_t*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_FLOAT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%f and ", \
                mysql_table.field[i].field_name, \
                *(float*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_CHAR_ARRAY){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s ='%s' and ", \
                mysql_table.field[i].field_name, \
                (char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else{
        fprintf(stderr, "unkown data_type on function '%s'\n", __FUNCTION__);
        return;
      }
    }
  }
  cmd_tmp[strlen(cmd_tmp)-4] = 0;
  sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s", "where ");
  for(i = 0; i < 32; i++){
    if((which & (1<<i)) != 0){
      if(mysql_table.field[i].data_type == DATA_TYPE_IS_CHAR){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ",  \
                mysql_table.field[i].field_name, \
                *(char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_UNCHAR){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ",  \
                mysql_table.field[i].field_name, \
                *(unsigned char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_INT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ", \
                mysql_table.field[i].field_name, \
                *(int32_t*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_UNINT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%u and ", \
                mysql_table.field[i].field_name, \
                *(uint32_t*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_FLOAT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%f and ", \
                mysql_table.field[i].field_name, \
                *(float*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_CHAR_ARRAY){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s ='%s' and ", \
                mysql_table.field[i].field_name, \
                (char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else{
        fprintf(stderr, "unkown data_type on function '%s'\n", __FUNCTION__);
        return;
      }
    }
  }
  cmd_tmp[strlen(cmd_tmp)-4] = 0;
  if(0 != mysql_real_query(mysql_fd, cmd_tmp, strlen(cmd_tmp))){
		fprintf(stderr, " mysql update record failed on function '%s'\n", __FUNCTION__);
	}
}

void mysql_demo_query_record(MYSQL* mysql_fd, mysql_table_t mysql_table, \
                             uint32_t which, mysql_demo_t* mysql_demo)
{
  MYSQL_STMT*  st_handle;
  MYSQL_BIND   para[mysql_table.filed_num];
  int32_t      i = 0;
  int32_t      status;
  int32_t      count;
  char         cmd_tmp[1024] ={0};

  if(0 != mysql_ping(mysql_fd)){return;}
  sprintf(&cmd_tmp[strlen(cmd_tmp)], "select * from %s where ", \
                                     mysql_table.table_name);
  for(i = 0; i < 32; i++){
    if((which & (1<<i)) != 0){
      if(mysql_table.field[i].data_type == DATA_TYPE_IS_CHAR){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ",  \
                mysql_table.field[i].field_name, \
                *(char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_UNCHAR){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ",  \
                mysql_table.field[i].field_name, \
                *(unsigned char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_INT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%d and ", \
                mysql_table.field[i].field_name, \
                *(int32_t*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_UNINT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%u and ", \
                mysql_table.field[i].field_name, \
                *(uint32_t*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_FLOAT){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s =%f and ", \
                mysql_table.field[i].field_name, \
                *(float*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else if(mysql_table.field[i].data_type == DATA_TYPE_IS_CHAR_ARRAY){
        sprintf(&cmd_tmp[strlen(cmd_tmp)], "%s ='%s' and ", \
                mysql_table.field[i].field_name, \
                (char*)(&mysql_demo + mysql_table.field[i].offset));
      }
      else{
        fprintf(stderr, "unkown data_type on function '%s'\n", __FUNCTION__);
        return;
      }
    }
  }

  for(i = 0; i < mysql_table.filed_num; i++){
    para[i].buffer_type   = mysql_table.field[i].buffer_type;
    para[i].buffer 		    = (void*)(mysql_demo + mysql_table.field[i].offset);
    para[i].buffer_length = mysql_table.field[i].buffer_length;
  }
  st_handle = mysql_stmt_init(mysql_fd);
  if (!st_handle){
    fprintf(stderr, " mysql initialize statement failed on function '%s'\n", __FUNCTION__);
    return;
  }
  status = mysql_stmt_prepare(st_handle, cmd_tmp, strlen(cmd_tmp));
  mysql_stmt_error_print(st_handle, status);
  status = mysql_stmt_bind_result(st_handle, para);
  mysql_stmt_error_print(st_handle, status);
  status = mysql_stmt_execute(st_handle);
  mysql_stmt_error_print(st_handle, status);
  status = mysql_stmt_store_result(st_handle);
  mysql_stmt_error_print(st_handle, status);
  count = mysql_stmt_num_rows(st_handle);
  for(i =1; i <count; i++)
  {
    mysql_stmt_fetch(st_handle);
    memcpy(&mysql_demo[i], &mysql_demo[0], sizeof(mysql_demo_t));
  }
  status = mysql_stmt_free_result(st_handle);
  mysql_stmt_error_print(st_handle, status);
  mysql_stmt_close(st_handle);
}
