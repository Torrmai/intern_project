#ifdef DATAINTERFACE
#include <sqlite3.h>
#include <stdio.h>
void conclude_stat(sqlite3 *db);
void update_data(sqlite3 *db,char *data);
int data_choice(sqlite3 *db,char *ip);
void insert_data(sqlite3 *db,char *ip);
#endif