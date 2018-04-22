/* dblayer.h - Database layer for ck -----------------------------------*- C -*-
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
#ifndef DBLAYER_H
#define DBLAYER_H

#include <sqlite3.h>

#include "actionparser.h"

typedef enum SqlErrors SqlError;
enum SqlErrors {
  SQL_NO_ERR = 0,
  SQL_ERR_NO_DB_FILE,
  SQL_ERR_NO_TABLES,
  SQL_ERR_SQLITE,
  SQL_CONFIG_PATH_EXISTS,
  SQL_ERR_PRIMARY_REDEFINITION
};

typedef struct DBstruct DB;
struct DBstruct {
  sqlite3 *ptr;
  SqlError error;
};

extern int db_exists(const UserOpt *opt);

/* Open the db file. On fail return null pointer to db 
 * and the corresponding SQL error (NO_DB_FILE | NO_TABLES)*/
extern DB open_DB(const UserOpt *opt);

extern void close_DB(DB *DB);

/********/
/* init */
/********/

/* Create the tables required for the ckdb */
extern void init_make_tables(DB *db);
extern DB init_make_DB(const UserOpt *opt);

/*******/
/* add */
/*******/

extern int add_transaction_begin(DB *db, const char *progName,
                                 const char* confPath, const int secret,
                                 const int prime);
extern int add_insert_program_to_db(DB *db, const char * name);
extern int add_insert_config_to_db(DB *db, const int pid, const char *path, const int secret, const int prime);
#endif /* DBLAYER_H */
