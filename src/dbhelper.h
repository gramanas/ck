/* dbhelper.h - Database layer for ck -----------------------------------*- C -*-
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
 * Help other db functions.
 *
 * -------------------------------------------------------------------------- */
#ifndef DBHELPER_H
#define DBHELPER_H

#include <sqlite3.h>

#include "actionparser.h"
#include "ckutil.h"

/********************/
/* sqlite constants */
/********************/
#define TBL_PROGRAM "PROGRAM"
#define TBL_CONFIG "CONFIG"
#define TBL_REL "REL"

#define COL_PROGRAM_ID "ID"
#define COL_PROGRAM_NAME "NAME"

#define COL_CONFIG_ID "ID"
#define COL_CONFIG_PATH "PATH"
#define COL_CONFIG_SECRET "SECRET"
#define COL_CONFIG_PRIMARY "PRIME"

#define COL_REL_PROGRAM_ID "PID"
#define COL_REL_CONFIG_ID "CID"

#define __BEGIN_TRANSACTION__ \
  sqlite3_exec(db->ptr, "BEGIN TRANSACTION;", NULL, NULL, NULL);
#define __END_TRANSACTION__ \
  sqlite3_exec(db->ptr, "END TRANSACTION;", NULL, NULL, NULL);

/****************/
/* form queries */
/****************/

extern void dbh_form_query_make_tables(char *query);
extern void dbh_form_query_insert_program(char *query);
extern void dbh_form_query_insert_config(char *query);
extern void dbh_form_query_select_id_from(char *query, const char* tableName);
extern void dbh_form_query_select_all_tables(char *query);
extern void dhb_form_query_insert_relationship(char *query);
extern void dhb_form_query_find_program(char *query);
extern void dhb_form_query_find_config(char *query);
extern void dhb_form_query_find_relationship(char *query);
extern void dbh_format_query_select_from_joined_eq(char *query, const char *selection, const char* condition);
extern void dbh_format_query_select_from_joined_like(char *query, const char *selection, const char* condition);
#endif /* DBHELPER_H */
