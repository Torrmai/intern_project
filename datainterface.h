<<<<<<< HEAD
#ifdef DATAINTERFACE
#include <sqlite3.h>
#include <stdio.h>
int i = 1;
int ch;
void conclude_stat(sqlite3 *db);
void update_data(sqlite3 *db,char *data);
int data_choice(sqlite3 *db,char *ip);
void insert_data(sqlite3 *db,char *ip);
static int callback_printdata(void *data,int argc,char **argv,char **azColName);
void create_tbl(sqlite3 *db);
#endif
=======

>>>>>>> c5cbf03263c6ef29a3951e7e0bf38b80fe686411
