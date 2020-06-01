#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
<<<<<<< HEAD
#include "datainterface.h"
=======
>>>>>>> c5cbf03263c6ef29a3951e7e0bf38b80fe686411
#define UPDATE 8
#define INSERT 9
int i = 1;
int ch;
static int callback_printdata(void *data,int argc,char **argv,char **azColName)
{
<<<<<<< HEAD

    for (int i = 0; i < argc; i++)
    {
        printf("|%s = %s|\t",azColName[i],argv[i] ? argv[i]:"NULL");
=======
    printf("data len: %d\n",argc);
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n",azColName[i],argv[i] ? argv[i]:"NULL");
>>>>>>> c5cbf03263c6ef29a3951e7e0bf38b80fe686411
    }
    printf("\n");
    return 0;
}
static int fetch_data(void *data,int argc,char **argv,char **colname){
   for(int i = 0;i<argc;i++){
<<<<<<< HEAD
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
   //temporary solution
   stat = sqlite3_exec(db,"delete from ip_stat",callback_printdata,0,&err);
   if(stat != SQLITE_OK){
      printf("err: %s\n",err);
   }
}
void update_data(sqlite3 *db,char *data){
   char com[100];
   com[0] = 0;
=======
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
void update_data(sqlite3 *db,char *data,char *com){
>>>>>>> c5cbf03263c6ef29a3951e7e0bf38b80fe686411
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
<<<<<<< HEAD
}
int data_choice(sqlite3 *db,char *ip){
   const char sqlcom[100];
=======
   else
   {
      printf("update to db sucessfully.....\n");
   }
   sprintf(com,"");
}
int data_choice(sqlite3 *db,char *ip,char *sqlcom){
>>>>>>> c5cbf03263c6ef29a3951e7e0bf38b80fe686411
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
<<<<<<< HEAD
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
=======
   else
   {
      printf("check ok\n");
   }
   sprintf(sqlcom,"");
   if(ch == 1){
      return INSERT;
   }
   else{
      return UPDATE;
   }
}
void insert_data(sqlite3 *db,char *sqlcom)
{
   int stat;
   char *err = 0;
>>>>>>> c5cbf03263c6ef29a3951e7e0bf38b80fe686411
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
<<<<<<< HEAD
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
=======
   }
}
int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sqlcom = (char *)calloc(100 ,sizeof(char));
   char *tmp = (char*)calloc(100,sizeof(char));
   const char* data = "wtf";
   rc = sqlite3_open("test.db", &db);

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
   
   while (1)
   {
      char *ip;
      scanf("%s",ip);
      int k = data_choice(db,ip,sqlcom);
      if (k == UPDATE)
      {
         update_data(db,ip,sqlcom);
      }
      else
      {
         sprintf(tmp,"insert into ip_stat (id,ip_addr,count)\n");
         strcat(sqlcom,tmp);
         sprintf(tmp,"select * from (select %d,'%s',1) as tmp\n",i,ip);
         strcat(sqlcom,tmp);
         sprintf(tmp,"where not exists (select ip_addr from ip_stat where ip_addr = '%s') limit 1\n",ip);
         strcat(sqlcom,tmp);
         insert_data(db,sqlcom);
         sprintf(sqlcom,"");
         sprintf(tmp,"");  
      }
   }
   sqlite3_close(db);
}
>>>>>>> c5cbf03263c6ef29a3951e7e0bf38b80fe686411
