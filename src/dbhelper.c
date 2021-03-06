/* dbhelper.c - Database layer for ck -----------------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 * 
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2018  Anastasis Grammenos
 * GPLv3 (see LICENCE for the full notice)
 *
 * -------------------------------------------------------------------------- */
#include "dbhelper.h"

void dbh_form_query_make_tables(char *query) {
  char tmp[STR_L] = "CREATE TABLE ";
  strcat(tmp, TBL_PROGRAM);
  strcat(tmp, "(");
  strcat(tmp, COL_PROGRAM_ID);
  strcat(tmp, " INT NOT NULL PRIMARY KEY, "); 
  strcat(tmp, COL_PROGRAM_NAME);
  strcat(tmp, " TEXT NOT NULL); ");

  strcat(tmp, "CREATE TABLE ");
  strcat(tmp, TBL_CONFIG);
  strcat(tmp, "(");
  strcat(tmp, COL_CONFIG_ID);
  strcat(tmp, " INT NOT NULL PRIMARY KEY, "); 
  strcat(tmp, COL_CONFIG_PATH);
  strcat(tmp, " TEXT NOT NULL, ");
  strcat(tmp, COL_CONFIG_SECRET);
  strcat(tmp, " INT NOT NULL, ");
  strcat(tmp, COL_CONFIG_PRIMARY);
  strcat(tmp, " INT NOT NULL); ");

  strcat(tmp, "CREATE TABLE ");
  strcat(tmp, TBL_REL);
  strcat(tmp, "(");
  strcat(tmp, COL_REL_PROGRAM_ID);
  strcat(tmp, " INT NOT NULL, "); 
  strcat(tmp, COL_REL_CONFIG_ID);
  strcat(tmp, " INT NOT NULL);");

  strcpy(query, tmp);
}

void dbh_form_query_insert_program(char *query) {
  char tmp[STR_L] = "INSERT INTO ";
  strcat(tmp, TBL_PROGRAM);
  strcat(tmp, " VALUES(?, ?);");

  strcpy(query, tmp);
}

void dbh_form_query_insert_config(char *query) {
  char tmp[STR_L] = "INSERT INTO ";
  strcat(tmp, TBL_CONFIG);
  strcat(tmp, " VALUES(?, ?, ?, ?);");

  strcpy(query, tmp);
}

void dbh_form_query_select_id_from(char *query, const char* tableName) {
  char tmp[STR_M] = "SELECT ID FROM ";
  strcat(tmp, tableName);
  strcat(tmp, " ORDER BY ID;");

  strcpy(query, tmp);
}

void dbh_form_query_select_all_tables(char *query) {
  strcpy(query, "SELECT * FROM SQLITE_MASTER WHERE type='table';");
}

void dhb_form_query_insert_relationship(char *query) {
  char tmp[STR_M] = "INSERT INTO ";
  strcat(tmp, TBL_REL);
  strcat(tmp, " VALUES(?, ?);");

  strcpy(query, tmp);
}

void dhb_form_query_find_program(char *query) {
  char tmp[STR_M] = "SELECT ID FROM ";
  strcat(tmp, TBL_PROGRAM);
  strcat(tmp, " WHERE ");
  strcat(tmp, COL_PROGRAM_NAME);
  strcat(tmp, " = ?;");

  strcpy(query, tmp);
}

void dhb_form_query_find_config(char *query) {
  char tmp[STR_M] = "SELECT ID FROM ";
  strcat(tmp, TBL_CONFIG);
  strcat(tmp, " WHERE ");
  strcat(tmp, COL_CONFIG_PATH);
  strcat(tmp, " = ?;");

  strcpy(query, tmp);
}

void dhb_form_query_find_relationship(char *query) {
  char tmp[STR_M] = "SELECT ";
  strcat(tmp, COL_REL_PROGRAM_ID);
  strcat(tmp, ", ");
  strcat(tmp, COL_REL_CONFIG_ID);
  strcat(tmp, " FROM ");
  strcat(tmp, TBL_REL);
  strcat(tmp, " WHERE ");
  strcat(tmp, COL_REL_PROGRAM_ID);
  strcat(tmp, " = ? AND ");
  strcat(tmp, COL_REL_CONFIG_ID);
  strcat(tmp, " = ?;");

  strcpy(query, tmp);
}

void dbh_form_query_select_from_joined_eq(char *query, const char *selection, const char* condition) {
  char tmp[STR_L] = "SELECT ";
  strcat(tmp, selection);
  strcat(tmp, " FROM ");
  strcat(tmp, TBL_REL);
  strcat(tmp, " JOIN ");
  strcat(tmp, TBL_PROGRAM);
  strcat(tmp, " ON ");
  strcat(tmp, TBL_PROGRAM);
  strcat(tmp, ".");
  strcat(tmp, COL_PROGRAM_ID);
  strcat(tmp, " = ");
  strcat(tmp, COL_REL_PROGRAM_ID);
  strcat(tmp, " JOIN ");
  strcat(tmp, TBL_CONFIG);
  strcat(tmp, " ON ");
  strcat(tmp, TBL_CONFIG);
  strcat(tmp, ".");
  strcat(tmp, COL_CONFIG_ID);
  strcat(tmp, " = ");
  strcat(tmp, COL_REL_CONFIG_ID);
  strcat(tmp, " WHERE ");
  strcat(tmp, condition);
  strcat(tmp, " = ?;");

  strcpy(query, tmp);
}

void dbh_format_query_select_from_joined_like(char *query, const char *selection, const char* condition) {
  char tmp[STR_L] = "SELECT ";
  strcat(tmp, selection);
  strcat(tmp, " FROM ");
  strcat(tmp, TBL_REL);
  strcat(tmp, " JOIN ");
  strcat(tmp, TBL_PROGRAM);
  strcat(tmp, " ON ");
  strcat(tmp, TBL_PROGRAM);
  strcat(tmp, ".");
  strcat(tmp, COL_PROGRAM_ID);
  strcat(tmp, " = ");
  strcat(tmp, COL_REL_PROGRAM_ID);
  strcat(tmp, " JOIN ");
  strcat(tmp, TBL_CONFIG);
  strcat(tmp, " ON ");
  strcat(tmp, TBL_CONFIG);
  strcat(tmp, ".");
  strcat(tmp, COL_CONFIG_ID);
  strcat(tmp, " = ");
  strcat(tmp, COL_REL_CONFIG_ID);
  strcat(tmp, " WHERE ");
  strcat(tmp, condition);
  strcat(tmp, " LIKE '%' || ? || '%';");

  strcpy(query, tmp);
}
