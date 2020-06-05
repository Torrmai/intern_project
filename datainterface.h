#ifdef DATAINTERFACE
#include <sqlite3.h>
#include <stdio.h>
#include <stdint.h>
int ch;
void conclude_stat(sqlite3 *db,char *target);
void update_data(sqlite3 *db,char *data,char *target,uint32_t pkt_size,uint16_t port);
int data_choice(sqlite3 *db,char *ip,char *target,uint16_t port);
void insert_data(sqlite3 *db,char *ip,char *target,uint16_t port,uint32_t pkt_size);
static int callback_printdata(void *data,int argc,char **argv,char **azColName);
void create_tbl(sqlite3 *db);
#endif
