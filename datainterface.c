#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include "datainterface.h"
#define UPDATE 8
#define INSERT 9
int i = 1;
int ch;
static int callback_printdata(void *data,int argc,char **argv,char **azColName)
{

    for (int i = 0; i < argc; i++)
    {
        printf("|%s = %s|\t",azColName[i],argv[i] ? argv[i]:"NULL");
    }
    printf("\n");
    return 0;
}
static int fetch_data(void *data,int argc,char **argv,char **colname){
   for(int i = 0;i<argc;i++){
      ch = atoi(argv[i]);
   }
   return 0;
}
void conclude_stat(sqlite3 *db){
   int stat;
   const char *data = "Rank: ";
   char *err = 0;
   char *comm = "select ip_addr,count from ip_stat "\
                "order by count DESC limit 10";
   stat = sqlite3_exec(db,comm,callback_printdata,0,&err);
   if(stat != SQLITE_OK){
      printf("err: %s\n",err);
   }
   stat = sqlite3_exec(db,"delete from ip_stat",callback_printdata,0,&err);
   if(stat != SQLITE_OK){
      printf("err: %s\n",err);
   }
}
void update_data(sqlite3 *db,char *data){
   char com[100];
   com[0] = 0;
   sprintf(com,"update ip_stat set count = count + 1 where ip_addr = '%s'",data);
   int stat;
   char *err = 0;
   stat = sqlite3_exec(db,com,callback_printdata,0,&err);
   if (stat != SQLITE_OK)
   {
      printf("sql err: %s\n",err);
      printf(com);
      sqlite3_free(err);
   }
}
int data_choice(sqlite3 *db,char *ip){
   const char sqlcom[100];
   int stat;
   char *err = 0;
   sprintf(sqlcom,"select count(*) from ip_stat where ip_addr = '%s'",ip);
   stat = sqlite3_exec(db,sqlcom,fetch_data,0,&err);
   if (stat != SQLITE_OK)
   {
      printf("sql err: %s\n",err);
      printf(sqlcom);
      sqlite3_free(err);
   }
   return ch;
}
void insert_data(sqlite3 *db,char *ip)
{
   char sqlcom[300];
   sqlcom[0] = 0;
   int stat;
   char *err = 0;
   sprintf(sqlcom,"insert into ip_stat (id,ip_addr,count) "\
               "select * from (select %d,'%s',1) as tmp "\
               "where not exists (select ip_addr from ip_stat where ip_addr = '%s') limit 1"\
               ,i,ip,ip);
   stat = sqlite3_exec(db,sqlcom,callback_printdata,0,&err);
   if (stat != SQLITE_OK)
   {
      printf("sql err: %s\n",err);
      printf(sqlcom);
      sqlite3_free(err);
   }
   else
   {
      i++;
   }  
}
void create_tbl(sqlite3 *db){
   const char comm[100];
   char *err = 0;
   int stat;
   sprintf(comm,"create table ip_stat("\
         "id int primary key not null,"\
         "ip_addr text not null unique,"\
         "count int not null)");
   stat = sqlite3_exec(db,comm,callback_printdata,0,&err);
   if (stat != SQLITE_OK)
   {
      printf("sql err: %s\n",err);
      sqlite3_free(err);
   }
   else{
      printf("May be it can create\n");
   }
}
