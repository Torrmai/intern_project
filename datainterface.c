#include <stdio.h>
#include <sys/stat.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "datainterface.h"
#define UPDATE 8
#define INSERT 9
int ch;
FILE *target_file;
static int callback_printdata(void *data,int argc,char **argv,char **azColName)
{

    for (int i = 0; i < argc; i++)
    {
        printf("| %s |\t",argv[i] ? argv[i]:"NULL");
    }
    printf("\n");
    return 0;
}
static int callback_printlog(void *data,int argc,char **argv,char **azColName)
{
   for(int i=0;i<argc;i++){
      if(i<argc-1){
         fprintf(target_file,"%s,",argv[i]?argv[i]:"NULL");
      }
      else{
         fprintf(target_file,"%s,",argv[i]?argv[i]:"NULL");
      }
   }
   fprintf(target_file,"\n");
   return 0;
}
static int fetch_data(void *data,int argc,char **argv,char **colname){
   for(int i = 0;i<argc;i++){
      ch = atoi(argv[i]);
   }
   return 0;
}
void calculate_matrix(sqlite3 *db){
   int stat;
   char *err = 0;
   stat = sqlite3_exec(db,"update ip_stat_src set pac_per_sec = count/60, tp = sum_tot_size/60",
                        callback_printdata,0,&err);
   if(stat != SQLITE_OK){
      printf("err: %s\n",err);
   }
   stat = sqlite3_exec(db,"update ip_stat_dst set pac_per_sec = count/60, tp = sum_tot_size/60",
                        callback_printdata,0,&err);
   if(stat != SQLITE_OK){
      printf("err: %s\n",err);
   }      
}
void create_log(sqlite3 *db){
   time_t t = time(NULL);
   struct stat st = {0};
   char path[100];
   char sep1[50];
   char sep2[50];
   char fileName[200];
   char linCom[110];
   int statt;
   char *err = 0;
   calculate_matrix(db);
   struct tm currtime = *localtime(&t);
   sprintf(path,"data/%d-%d/%d/%d",currtime.tm_mon,(currtime.tm_year+1900),currtime.tm_mday,currtime.tm_hour);
   sprintf(sep1,"data/%d-%d",currtime.tm_mon,(currtime.tm_year+1900));
   sprintf(sep2,"data/%d-%d/%d",currtime.tm_mon,(currtime.tm_year+1900),currtime.tm_mday);
   if(stat(path,&st)== -1){
      mkdir("data",0755);
      mkdir(sep1,0755);
      mkdir(sep2,0755);
      mkdir(path,0755);
      printf("Create folder...\n");
   }
   sprintf(fileName,"%s/%d:%d.csv",
            path,currtime.tm_min,currtime.tm_sec);
   target_file = fopen(fileName,"w+");
   if(target_file == NULL){
      printf("Null file can't create file....\n");
      exit(0);
   }else{
      fprintf(target_file,"-------record at %d:%d:%d------\n",currtime.tm_hour,currtime.tm_min,currtime.tm_sec);
      // fprintf(target_file,"-------format col1 ip\tcol2 port\tcol3 count\tcol4 sum of frame size------\n");
      fprintf(target_file,"-------------ip src statistic---------\n");
      char *comm = "select * from ip_stat_src "\
                   "order by count DESC";
      statt = sqlite3_exec(db,comm,callback_printlog,0,&err);
      if(statt != SQLITE_OK){
         printf("err: %s\n",err);
      }
      fprintf(target_file,"-------------ip dst statistic---------\n");
      char *comm2 = "select * from ip_stat_dst "\
                    "order by count DESC";
      statt = sqlite3_exec(db,comm2,callback_printlog,0,&err);
      if(statt != SQLITE_OK){
         printf("err: %s\n",err);
      }
      printf("Log file: %s has been created successfully\n",fileName);
      fclose(target_file);
      //Now it's like triger function
      statt = sqlite3_exec(db,"delete from ip_stat_src",callback_printdata,0,&err);
      if(statt != SQLITE_OK){
         printf("err: %s\n",err);
      }
      statt = sqlite3_exec(db,"delete from ip_stat_dst",callback_printdata,0,&err);
      if(statt != SQLITE_OK){
         printf("err: %s\n",err);
      }
   }
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
   sprintf(com,"update ip_stat_%s set count = count + 1.0,sum_tot_size = sum_tot_size + %d where ip_addr = '%s' and port = %d",target,pkt_size,data,port);
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
               "select * from (select '%s',%d,1.0,%ld) as tmp "\
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
         "count double not null,"\
         "sum_tot_size double not null,"\
         "pac_per_sec double,"\
         "tp double,"\
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
         "count double not null,"\
         "sum_tot_size double not null,"\
         "pac_per_sec double,"\
         "tp double,"\
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
