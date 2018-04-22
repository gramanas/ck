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
#include "dblayer.h"
#include "dbhelper.h"
#include "ckutil.h"

const char * const DB_NAME = "/ckdb";

/* figure out the database name */
void make_db_name(char *ret, const char *confPath) {
  char db_path[STR_L];
  strcpy(db_path, confPath);
  strcat(db_path, DB_NAME);

  strcpy(ret, db_path);
}

/* Check if the db file exists*/
int db_exists(const UserOpt *opt) {
  char db_path[STR_L];
  make_db_name(db_path, opt->confDir);
  return util_is_file_rw(db_path);
}

/* check if db has the correct tables */
int check_initialized_DB(sqlite3 *db) {
  char sql[STR_M];
  dbh_form_query_select_all_tables(sql);
  sqlite3_stmt *stmt;

  int rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

  int program_table_ok, config_table_ok, rel_table_ok = 0;
  while (sqlite3_step(stmt) != SQLITE_DONE) {
    const unsigned char *tmpbuf  = sqlite3_column_text(stmt, 1);
    if (strcmp((char *)tmpbuf, TBL_PROGRAM) == 0) {
      program_table_ok = 1;
    }
    if (strcmp((char *)tmpbuf, TBL_CONFIG) == 0) {
      config_table_ok = 1;
    }
    if (strcmp((char *)tmpbuf, TBL_REL) == 0) {
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
  char db_path[STR_L];
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
  char db_path[STR_L];

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
  char sql[STR_L];
  dbh_form_query_make_tables(sql);

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

  char sql[STR_M];
  dbh_form_query_select_id_from(sql, tableName);
  
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

int insert_to_program_table(DB *db, const char *name) {
  sqlite3_stmt *stmt;
  int rc;

  char sql[STR_L];
  dbh_form_query_insert_program(sql);

  rc = sqlite3_prepare_v2(db->ptr, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    printf("Error while preparing insert to program sql.\n");
    db->error = SQL_ERR_SQLITE;    
    return -1;
  }
  int id = get_next_valid_id_from_table(db, TBL_PROGRAM);
  if (id == -1) {
    db->error = SQL_ERR_SQLITE;
    return -1;
  }
  sqlite3_bind_int(stmt, 1, id);
  sqlite3_bind_text(stmt, 2, name, strlen(name), 0);
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    printf("Error while excecuting insert to program sql.\n");
    db->error = SQL_ERR_SQLITE;
    return -1;
  }
  sqlite3_finalize(stmt);
  return id;
}

int insert_to_config_table(DB *db, const char *path, const int secret, const int prime) {
  sqlite3_stmt *stmt;
  int rc;

  char sql[STR_L];
  dbh_form_query_insert_config(sql);
  rc = sqlite3_prepare_v2(db->ptr, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    printf("Error while preparing insert to config sql.\n");
    db->error = SQL_ERR_SQLITE;
    return -1;
  }
  int id = get_next_valid_id_from_table(db, TBL_CONFIG);
  if (id == -1) {
    db->error = SQL_ERR_SQLITE;
    return -1;
  }
  sqlite3_bind_int(stmt, 1, id);
  sqlite3_bind_text(stmt, 2, path, strlen(path), 0);
  sqlite3_bind_int(stmt, 3, secret);
  sqlite3_bind_int(stmt, 4, prime);
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    printf("Error while excecuting insert to config sql.\n");
    db->error = SQL_ERR_SQLITE;
    return-1;
  }
  sqlite3_finalize(stmt);
  return id;
}

int insert_to_rel_table(DB *db, const int pid, const int cid) {
  sqlite3_stmt *stmt;
  int rc;

  char sql[STR_M];
  dhb_form_query_insert_relationship(sql);
  rc = sqlite3_prepare_v2(db->ptr, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    db->error = SQL_ERR_SQLITE; 
    printf("Error while preparing insert to rel sql.\n");
    return -1;
  }
  sqlite3_bind_int(stmt, 1, pid);
  sqlite3_bind_int(stmt, 2, cid);
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    db->error = SQL_ERR_SQLITE; 
    printf("Error while excecuting insert to rel sql.\n");
    return-1;
  }
  sqlite3_finalize(stmt);
  return 1;
}

int program_exists(DB *db, const char* name) {
  sqlite3_stmt *stmt;
  int rc;

  char sql[STR_M];
  dhb_form_query_find_program(sql);

  rc = sqlite3_prepare_v2(db->ptr, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    printf("Error while preparing program_exists sql.\n");
    return -2;
  }
  sqlite3_bind_text(stmt, 1, name, strlen(name), 0);
  int id = -1;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    id = sqlite3_column_int(stmt, 0);
    break;
  }
  sqlite3_finalize(stmt);
  return id;
}

int config_exists(DB *db, const int pid, const char* path) {
  sqlite3_stmt *stmt;
  int rc;

  char sql[STR_M];
  dhb_form_query_find_config(sql);

  rc = sqlite3_prepare_v2(db->ptr, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    printf("Error while preparing config_exists sql.\n");
    return -2;
  }
  sqlite3_bind_text(stmt, 1, path, strlen(path), 0);
  int id = -1;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    id = sqlite3_column_int(stmt, 0);
    break;
  }
  sqlite3_finalize(stmt);
  return id;
}

int add_insert_relationship(DB *db, const int pid, const int cid) {
  // do checks
  return insert_to_rel_table(db, pid, cid);
}

int add_get_or_insert_config_to_db(DB *db, const int pid, const char *path, const int secret, const int prime) {
  int cid = config_exists(db, pid, path);
  if (cid == -2) {
    db->error = SQL_ERR_SQLITE; 
    return -1;
  }
  /* If config doesnt exist insert it and return it's cid */
  if (cid == -1) {
    return insert_to_config_table(db, path, secret, prime);
  }

  /* If it exist it means the user has inserted the same path twice */
  db->error = SQL_CONFIG_PATH_EXISTS;
  return -1;
}

int add_get_or_insert_program_to_db(DB *db, const char *name) {
  int pid = program_exists(db, name);
  if (pid == -2) {
    db->error = SQL_ERR_SQLITE; 
    return -1;
  }
  if (pid == -1) {
    return insert_to_program_table(db,name);
  }
  return pid;
}

int add_transaction_begin(DB *db, const char *progName,
                          const char* confPath, const int secret,
                          const int prime) {
  __BEGIN_TRANSACTION__
  int pid = add_get_or_insert_program_to_db(db, progName);
  if (db->error == SQL_ERR_SQLITE) {
    printf("Could not insert program to db.\n");
    close_DB(db);
    return 0;
  }
  int cid = add_get_or_insert_config_to_db(db, pid, confPath, secret, prime);
  if (db->error == SQL_ERR_SQLITE) {
    printf("Could not insert config to db.\n");
    close_DB(db);
    return 0;
  }
  else if (db->error == SQL_CONFIG_PATH_EXISTS) {
    printf("This config already exists in the database.\n");
    close_DB(db);
    return 0;
  }
  add_insert_relationship(db, pid, cid);
  if (db->error == SQL_ERR_SQLITE) {
    printf("Could not insert config to db.\n");
    close_DB(db);
    return 0;
  }
  __END_TRANSACTION__

  return 1;
}
