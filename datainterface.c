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
        printf("| %s |\t",argv[i] ? argv[i]:"NULL");
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
void conclude_stat(sqlite3 *db,char *target){
   int stat;
   const char *data = "Rank: ";
   char *err = 0;
   // char *comm = "select * from ip_stat_src "\
   //              "order by count DESC limit 10";
   char comm[300];
   sprintf(comm,"select * from ip_stat_%s "\
         "order by count DESC limit 10",target);
   printf("| ip |\t| port |\t| count |\t| sum size |\n");
   stat = sqlite3_exec(db,comm,callback_printdata,0,&err);
   if(stat != SQLITE_OK){
      printf("err: %s\n",err);
   }
   //temporary solution
   /*
   stat = sqlite3_exec(db,"delete from ip_stat",callback_printdata,0,&err);
   if(stat != SQLITE_OK){
      printf("err: %s\n",err);
   }*/
}
void update_data(sqlite3 *db,char *data,char *target,uint32_t pkt_size,uint16_t port){
   char com[200];
   com[0] = 0;
   sprintf(com,"update ip_stat_%s set count = count + 1,sum_tot_size = sum_tot_size + %d where ip_addr = '%s' and port = %d",target,pkt_size,data,port);
   int stat;
   char *err = 0;
   stat = sqlite3_exec(db,com,callback_printdata,0,&err);
   if (stat != SQLITE_OK)
   {
      printf("can't update\n");
      printf("sql err: %s\n",err);
      //printf(com);
      sqlite3_free(err);
   }
}
int data_choice(sqlite3 *db,char *ip,char *target,uint16_t port){
   const char sqlcom[200];
   int stat;
   char *err = 0;
   sprintf(sqlcom,"select count(*) from ip_stat_%s where ip_addr = '%s' and port = %d",target,ip,port);
   stat = sqlite3_exec(db,sqlcom,fetch_data,0,&err);
   if (stat != SQLITE_OK)
   {
      printf("can't make a choice!!!!\n");
      printf("sql err: %s\n",err);
      //printf(sqlcom);
      sqlite3_free(err);
   }
   return ch;
}
void insert_data(sqlite3 *db,char *ip,char *target,uint16_t port,uint32_t pkt_size)
{
   char sqlcom[300];
   sqlcom[0] = 0;
   int stat;
   char *err = 0;
   sprintf(sqlcom,"insert into ip_stat_%s (ip_addr,port,count,sum_tot_size) "\
               "select * from (select '%s',%d,1,%ld) as tmp "\
               "where not exists (select ip_addr from ip_stat_%s where ip_addr = '%s' and port = %d) limit 1"\
               ,target,ip,port,pkt_size,target,ip,port);
   stat = sqlite3_exec(db,sqlcom,callback_printdata,0,&err);
   if (stat != SQLITE_OK)
   {
      printf("Error occur at insert\n");
      printf("sql err: %s\n",err);
      //printf(sqlcom);
      sqlite3_free(err);
   }
}
void create_tbl(sqlite3 *db){
   const char comm[300];
   char *err = 0;
   int stat;
   sprintf(comm,"create table ip_stat_src("\
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
      printf("Create ip_stat_src\n");
   }
   sprintf(comm,"create table ip_stat_dst("\
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
      printf("Create ip_stat_dst\n");
   }
}
