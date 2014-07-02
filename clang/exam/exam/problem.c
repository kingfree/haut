/* problem.c
 * 题目数据库的增删改查
 */

FILE * problem_open()
{
  return db_open(problem_db_name);
}

int problem_close(FILE *db)
{
  return db_close(db);
}

int problem_insert(FILE *db, problem *p)
{
  return db_insert(db, p);
}
