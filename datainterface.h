#ifdef DATAINTERFACE
#include <sqlite3.h>
#include <stdio.h>
void conclude_stat(sqlite3 *db);
void update_data(sqlite3 *db,char *data,char *com);
int data_choice(sqlite3 *db,char *ip,char *sqlcom);
void insert_data(sqlite3 *db,char *sqlcom,char *tmp,char *ip);
#endif