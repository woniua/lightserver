#define EXT_MYSQL_OPTIONS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "mysql_options.h"
#include "config.h"

{
        char err_buf[1024] = {0};
        if(status)
        {
                fprintf(stderr, "Error: %s (errno: %d)\n", mysql_stmt_error(stmt), mysql_stmt_errno(stmt));
                sprintf(err_buf, "Error: %s (errno: %d)\n", mysql_stmt_error(stmt), mysql_stmt_errno(stmt));
                log_record(err_buf, LOG_TYPE_MYSQL, "mysql_err");
        }
}

void IDD_mysql_option_set(MYSQL* mysql_handle)
{
        //enable auto-reconnent
        //set max time-out
}


/******************************************************************************
 * prototype: IDD_mysql_connect_init()
 *brief     :
 * para in  :
 * para out :
 * return   :
 * note     :database always actived.
 ******************************************************************************/
void IDD_mysql_connect_init(void)
{
        my_bool reconnect = 1;//Ê¹ÄÜ×Ô¶¯ÖØÁ¬
        int i;

        for(i =0; i< MYSQL_CONN_MAX; i++){
                mysql_object[i].mysql_fd = mysql_init(&(mysql_object[i].mysql));
                mysql_options(mysql_object[i].mysql_fd, MYSQL_OPT_RECONNECT, &reconnect);
                if( NULL != mysql_real_connect(mysql_object[i].mysql_fd,"grc2north.mysqldb.chinacloudapi.cn", "grc2north%dba","R\"ljPX)3$y9CoLSY","default",0,0,0) ){
                //if( NULL != mysql_real_connect(mysql_object[i].mysql_fd,"localhost","root","123456","mysql",0,0,0) ){
                        printf("Mysql connection total %d success!\r",i+1);
                }
                else{
                        printf("connection error: %s\n", mysql_error(mysql_object[i].mysql_fd) );
                        exit(1);
                }
        }
        printf("\nMysql connection success!\n");
}



/******************************************************************************
 * prototype: IDD_mysql_create_table()
 *brief     :ŽŽœšÊýŸÝ¿â±í
 * para in  :"tablename"----ŽŽœšµÄ±íÃû
             "..."----------¿É±ä²ÎÊýÁÐ±í£¬ŽŽœšµÄ±íµÄž÷×Ö¶ÎÃûºÍÀàÐÍ
 * para out :
 * return   :ŽŽœš³É¹Š·µ»Ø0£¬ Ê§°Ü·µ»Ø-1
 * note     :µ÷ÓÃÊ±È·±£×îºóÒ»žö²ÎÊýÎª"NULL"
 ******************************************************************************/

int IDD_mysql_create_table(MYSQL* mysql_handle, char* tablename, ...)
{
        char create_table[1024] ={0};
        char arg_buf[788]       ={0};
        char* argp =NULL;
        va_list args;
        int rtn =0;

        va_start(args, tablename);
        while(1)
        {
                argp =va_arg(args, char*);
                if(argp != NULL){
                        strcat(&arg_buf[strlen(arg_buf)], argp);
                        strcat(&arg_buf[strlen(arg_buf)], ", ");
                }
                else{
                        arg_buf[strlen(arg_buf) -2] =0;
                        break;
                }
        }
        va_end(args);
     sprintf(create_table,"create table IF NOT EXISTS %s (%s)", tablename, arg_buf);
        if(0 == mysql_ping(mysql_handle))//connection is actived
        {
                if( 0 == mysql_real_query(mysql_handle,create_table,strlen(create_table)) ){//create table successed
                        printf("create table %s success!\n", tablename);
                }
                else{
                        rtn = -1;
                        printf("create table %s failed!\n", tablename);
                }
        }else{
                rtn = -1;
        }

        return rtn;
}




int IDD_mysql_usrpasswd_create_table(MYSQL* mysql_handle)
{
        return IDD_mysql_create_table(mysql_handle, "usrpasswd", "usrname char(8)", \
                                                    "passwd char(8)", \

    "register_date char(13)", \

    "login_date char(13)",\

    "company_name char(32)", NULL);
}
