/* db.h
 * 数据库的增删改查
 */

#ifndef _PROBLEM_H_
#define _PROBLEM_H_

FILE * db_open(char *db_name);
int db_close(FILE *db);

int db_insert(FILE *db, void *p);
int db_delete(FILE *db, void *p);
int db_update(FILE *db, void *p);
int db_select(FILE *db, void *p);

int db_count(FILE *db, void *p);

#endif
