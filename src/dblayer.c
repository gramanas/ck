/* dblayer.c - Database layer for ck ---------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * ------------------------------------------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dblayer.h"

int db_exists() {
  FILE *db_file;

  if ((db_file = fopen("/home/gramanas/.ck/ckdb", "rb")) == NULL) {
    return 0;
  }
  fclose(db_file);
  return 1;
}

// check if db has the correct tables
int check_initialized_DB(sqlite3 *db) {
  char *sql = "SELECT * FROM SQLITE_MASTER;";
  sqlite3_stmt *stmt;

  int rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

  int program_table_ok, config_table_ok, rel_table_ok = 0;
  while (sqlite3_step(stmt) != SQLITE_DONE) {
    const unsigned char *tmpbuf  = sqlite3_column_text(stmt, 1);
    if (strcmp((char *)tmpbuf, "PROGRAM") == 0) {
      program_table_ok = 1;
    }
    if (strcmp((char *)tmpbuf, "CONFIG") == 0) {
      config_table_ok = 1;
    }
    if (strcmp((char *)tmpbuf, "REL") == 0) {
      rel_table_ok = 1;
    }
  }

  if (!program_table_ok
      || !config_table_ok
      || !rel_table_ok ) {
    return 1;
  }

  sqlite3_finalize(stmt);
  return 0;
}

DB empty_DB(SqlError err) {
  return (DB){ .ptr = NULL, .error = err };
}

DB new_DB(sqlite3 *db) {
  return (DB){ .ptr = db, .error = SQL_NO_ERR };
}

void close_DB(DB *db) {
  sqlite3_close(db->ptr);
}

DB init_make_DB() {
  sqlite3 *db;
  int rc;

  rc = sqlite3_open("/home/gramanas/.ck/ckdb", &db);

  if (rc) {
    return empty_DB(SQL_ERR_NO_DB_FILE);
  }

  return new_DB(db);
}

DB open_DB() {
  sqlite3 *db;
  int rc;

  rc = sqlite3_open("/home/gramanas/.ck/ckdb", &db);

  if (rc) {
    return empty_DB(SQL_ERR_NO_DB_FILE);
  }

  if (check_initialized_DB(db)) {
    return empty_DB(SQL_ERR_NO_TABLES);
  }

  return new_DB(db);
}

void init_make_tables(DB *db) {
  char * sql = "CREATE TABLE REL("
    "pId INT    NOT NULL,"
    "cId INT    NOT NULL);"
    "CREATE TABLE PROGRAM("
    "id INT PRIMARY KEY     NOT NULL,"
    "name           TEXT    NOT NULL);"
    "CREATE TABLE CONFIG("
    "id INT PRIMARY KEY     NOT NULL,"
    "path           TEXT    NOT NULL,"
    "secret         INT     NOT NULL,"
    "prime          INT     NOT NULL);";

  char *err_msg = NULL;
  int rc = sqlite3_exec(db->ptr, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK ) {
    printf("SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    db->error = SQL_ERR_SQLITE;
    return;
  }
}
