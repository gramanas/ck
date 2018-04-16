/* dblayer.h - Database layer for ck ---------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * ---------------------------------------------------------------------
 *
 *
 * ------------------------------------------------------------------ */
#ifndef DBLAYER_H
#define DBLAYER_H

#include <sqlite3.h>

#include "actionparser.h"

typedef enum SqlErrors SqlError;
enum SqlErrors {
  SQL_NO_ERR = 0,
  SQL_ERR_NO_DB_FILE,
  SQL_ERR_NO_TABLES,
  SQL_ERR_SQLITE
};

typedef struct DBstruct DB;
struct DBstruct {
  sqlite3 *ptr;
  SqlError error;
};

extern int db_exists(UserOpt *opt);
extern DB open_DB();
extern void close_DB(DB *DB);

/* init */
extern void init_make_tables(DB *db);
extern DB init_make_DB();

#endif /* DBLAEYR_H */
