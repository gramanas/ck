/* dblayer.c - Database layer for ck -----------------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 * 
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2018  Anastasis Grammenos
 * GPLv3 (see LICENCE for the full notice)
 *
 * -----------------------------------------------------------------------------
 *
 * Give access to the database.
 *
 * -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dblayer.h"
#include "ckutil.h"

const char * const DB_NAME = "/ckdb";

/* figure out the database name */
void make_db_name(char *ret, const char *confPath) {
  char db_path[200];
  strcpy(db_path, confPath);
  strcat(db_path, DB_NAME);

  strcpy(ret, db_path);
}

/* Check if the db file exists*/
int db_exists(const UserOpt *opt) {
  char db_path[200];
  make_db_name(db_path, opt->confDir);
  return util_is_file_rw(db_path);
}

/* check if db has the correct tables */
int check_initialized_DB(sqlite3 *db) {
  char *sql = "SELECT * FROM SQLITE_MASTER WHERE type='table';";
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

DB init_make_DB(const UserOpt *opt) {
  sqlite3 *db;
  char db_path[200];
  int rc;

  make_db_name(db_path, opt->confDir);
  rc = sqlite3_open(db_path, &db);
  if (rc != SQLITE_OK) {
    return empty_DB(SQL_ERR_NO_DB_FILE);
  }

  return new_DB(db);
}

DB open_DB(const UserOpt *opt) {
  sqlite3 *db;
  int rc;
  char db_path[200];

  make_db_name(db_path, opt->confDir);  
  rc = sqlite3_open(db_path, &db);

  if (rc) {
    return empty_DB(SQL_ERR_NO_DB_FILE);
  }

  if (check_initialized_DB(db)) {
    return empty_DB(SQL_ERR_NO_TABLES);
  }

  return new_DB(db);
}

void init_make_tables(DB *db) {
  char * sql =
    "CREATE TABLE REL("
    "pId INT NOT NULL,"
    "cId INT NOT NULL);"
    "CREATE TABLE PROGRAM("
    "id   INT  NOT NULL PRIMARY KEY,"
    "name TEXT NOT NULL);"
    "CREATE TABLE CONFIG("
    "id     INT  NOT NULL PRIMARY KEY,"
    "path   TEXT NOT NULL,"
    "secret INT  NOT NULL,"
    "prime  INT  NOT NULL);";

  char *err_msg = NULL;
  int rc = sqlite3_exec(db->ptr, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK ) {
    printf("SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    db->error = SQL_ERR_SQLITE;
    return;
  }
}

int get_next_valid_id_from_table(DB *db, const char* tableName) {
  sqlite3_stmt *stmt;
  int rc;

  char sql[100] = "SELECT id FROM ";
  strcat(sql, tableName);
  strcat(sql, " ORDER BY id;");

  rc = sqlite3_prepare_v2(db->ptr, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    return -1;
  }
  sqlite3_bind_text(stmt, 1, tableName, strlen(tableName), 0);

  int id = 0;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    int a = sqlite3_column_int(stmt, 0);
    if (a != id) {
      break;
    }
    id++;
  }
  sqlite3_finalize(stmt);
  return id;
}

void insert_to_program_table(DB *db, const char *name) {
  sqlite3_stmt *stmt;
  int rc;

  char * sql =
    "INSERT INTO PROGRAM VALUES(?, ?);";

  rc = sqlite3_prepare_v2(db->ptr, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    printf("Error\n");
    return;
  }
  int id = get_next_valid_id_from_table(db, "PROGRAM");
  if (id == -1) {
    db->error = SQL_ERR_SQLITE;
    return;
  }
  sqlite3_bind_int(stmt, 1, id);
  sqlite3_bind_text(stmt, 2, name, strlen(name), 0);
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    printf("Error\n");
    return;
  }
  sqlite3_finalize(stmt);
}

int add_insert_program_to_db(DB *db, const char *name) {
  insert_to_program_table(db,name);
  return 0;
}
