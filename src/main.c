#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h> 

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for(i = 0; i<argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

void initDb(sqlite3 *db) {
  char *zErrMsg = 0;
  char *sql;
  int rc;

  /* Create SQL statement */
  sql = "CREATE TABLE PROGRAM("                  \
    "id INT PRIMARY KEY     NOT NULL,"           \
    "name           TEXT    NOT NULL);";
  
  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

  if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  } else {
    fprintf(stdout, "Table created successfully\n");
  }  
  sql = "CREATE TABLE CONFIG("                   \
    "id INT PRIMARY KEY     NOT NULL,"           \
    "path           TEXT    NOT NULL,"           \
    "secret         INT     NOT NULL,"           \
    "prime          INT     NOT NULL);";

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

  if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  } else {
    fprintf(stdout, "Table created successfully\n");
  }
  sql = "CREATE TABLE REL("                      \
    "pId INT    NOT NULL,"                       \
    "cId INT    NOT NULL);";
  
  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

  if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  } else {
    fprintf(stdout, "Table created successfully\n");
  }   
}

 int main(int argc, char* argv[]) {
  sqlite3 *db;
  int rc;

  /* Open database */
  rc = sqlite3_open("ckdb", &db);
   
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return(0);
  }
  else {
    fprintf(stdout, "Opened database successfully\n");
  }

  if (strcmp("init", argv[1]) == 0) {
    initDb(db);
  }

  sqlite3_close(db);
  return 0;
}
