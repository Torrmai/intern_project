#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#define UPDATE 8
#define INSERT 9
int i = 1;
int ch;
static int callback_printdata(void *data,int argc,char **argv,char **azColName)
{
    printf("data len: %d\n",argc);
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n",azColName[i],argv[i] ? argv[i]:"NULL");
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
void update_data(sqlite3 *db,char *data,char *com){
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
int data_choice(sqlite3 *db,char *ip,char *sqlcom){
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