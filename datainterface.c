#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "datainterface.h"
#define UPDATE 8
#define INSERT 9
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
   char *comm = "select * from ip_stat "\
                "order by count DESC limit 10";
   stat = sqlite3_exec(db,comm,callback_printdata,0,&err);
   if(stat != SQLITE_OK){
      printf("err: %s\n",err);
   }
   //temporary solution
   stat = sqlite3_exec(db,"delete from ip_stat",callback_printdata,0,&err);
   if(stat != SQLITE_OK){
      printf("err: %s\n",err);
   }
}
void update_data(sqlite3 *db,char *data,uint32_t pkt_size,uint16_t port){
   char com[200];
   com[0] = 0;
   sprintf(com,"update ip_stat set count = count + 1,sum_tot_size = sum_tot_size + %d where ip_addr = '%s' and port = %d",pkt_size,data,port);
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
int data_choice(sqlite3 *db,char *ip,uint16_t port){
   const char sqlcom[200];
   int stat;
   char *err = 0;
   sprintf(sqlcom,"select count(*) from ip_stat where ip_addr = '%s' and port = %d",ip,port);
   stat = sqlite3_exec(db,sqlcom,fetch_data,0,&err);
   if (stat != SQLITE_OK)
   {
      printf("sql err: %s\n",err);
      printf(sqlcom);
      sqlite3_free(err);
   }
   return ch;
}
void insert_data(sqlite3 *db,char *ip,uint16_t port,uint32_t pkt_size)
{
   char sqlcom[300];
   sqlcom[0] = 0;
   int stat;
   char *err = 0;
   sprintf(sqlcom,"insert into ip_stat (ip_addr,port,count,sum_tot_size) "\
               "select * from (select '%s',%d,1,%ld) as tmp "\
               "where not exists (select ip_addr from ip_stat where ip_addr = '%s' and port = %d) limit 1"\
               ,ip,port,pkt_size,ip,port);
   stat = sqlite3_exec(db,sqlcom,callback_printdata,0,&err);
   if (stat != SQLITE_OK)
   {
      printf("sql err: %s\n",err);
      printf(sqlcom);
      sqlite3_free(err);
   }
}
void create_tbl(sqlite3 *db){
   const char comm[300];
   char *err = 0;
   int stat;
   sprintf(comm,"create table ip_stat("\
         "ip_addr text not null,"\
         "port int not null,"\
         "count int not null,"\
         "sum_tot_size int not null,"\
         "primary key (ip_addr,port))");
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
