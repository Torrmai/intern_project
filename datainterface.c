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
        printf("\t--->%s = %s\n",azColName[i],argv[i] ? argv[i]:"NULL");
    }
    printf("\n");
    return 0;
}
static int fetch_data(void *data,int argc,char **argv,char **colname){
   for(int i = 0;i<argc;i++){
      printf("%s\n",argv[i]);
      if(atoi(argv[i]) != 0){
         ch = 0;
      }
      else{
         ch = 1;
      }
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
}
void update_data(sqlite3 *db,char *data){
   const char* com[100];
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
   else
   {
      printf("update to db sucessfully.....\n");
   }
   sprintf(com,"");
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
   else
   {
      printf("check ok\n");
   }
   sprintf(sqlcom,"");
   if(ch == 1){
      insert_data(db,ip);
   }
   else{
      update_data(db,ip);
   }
}
void insert_data(sqlite3 *db,char *ip)
{
   const char sqlcom[100];
   const char tmp[100];
   int stat;
   char *err = 0;
   sprintf(tmp,"insert into ip_stat (id,ip_addr,count)\n");
   strcat(sqlcom,tmp);
   sprintf(tmp,"select * from (select %d,'%s',1) as tmp\n",i,ip);
   strcat(sqlcom,tmp);
   sprintf(tmp,"where not exists (select ip_addr from ip_stat where ip_addr = '%s') limit 1\n",ip);
   strcat(sqlcom,tmp);
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
