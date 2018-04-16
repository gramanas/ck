/* actions.c - ck actions --------------------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * ------------------------------------------------------------------ */
#include <stdio.h>

#include "actions.h"
#include "dblayer.h"


int run_INIT(UserOpt * opt, Conf *conf) {
  if (db_exists(opt)) {
    printf("conf dir: %s\n", opt->confDir);
    printf("ck is already initialized.\n");
    return 0;
  }
  if (init_create_config_file(opt)) {
    return 0;
  }
  DB db = init_make_DB(opt);
  if (db.error == SQL_NO_ERR) {
    init_make_tables(&db);
  }
  sqlite3_close(db.ptr);
  return 1;
}

int run_ADD(UserOpt * opt, Conf *conf) {
  printf("Running %s\n", "add");
  DB db = open_DB();
  if (db.ptr == NULL) {
    if (db.error == SQL_ERR_NO_TABLES) {
      printf("no tables\n");
    }
  }
  for (int i = 0; i < opt->argc; i++) {
    printf("[%d]: %s\n", i, opt->argv[i]);
  }
  return 0;
}

int run_DEL(UserOpt * opt, Conf *conf) {
  printf("Running %s\n", "del");
  return 0;
}

int run_EDIT(UserOpt * opt, Conf *conf) {
  printf("Running %s\n", "edit");
  return 0;
}

int run_LIST(UserOpt * opt, Conf *conf) {
  printf("Running %s\n", "list");
  return 0;
}

int run_SEARCH(UserOpt * opt, Conf *conf) {
  printf("Running %s\n", "search");
  return 0;
}

int run_HELP(UserOpt * opt, Conf *conf) {
  printf("Running %s\n", "help");
  return 0;
}

void print_INIT_result(int ok) {
  if (ok) {
    printf("Initialized empty ckdb.\n");
    return;
  }
}

void print_ADD_result(int ok) {
  if (ok) {
    printf("succes\n");
    return;
  }
  printf("failure\n");
}

void print_DEL_result(int ok) {
  if (ok) {
    printf("succes\n");
    return;
  }
  printf("failure\n");
}

void print_EDIT_result(int ok) {
  if (ok) {
    printf("succes\n");
    return;
  }
  printf("failure\n");
}

void print_LIST_result(int ok) {
  if (ok) {
    printf("succes\n");
    return;
  }
  printf("failure\n");
}

void print_SEARCH_result(int ok) {
  if (ok) {
    printf("succes\n");
    return;
  }
  printf("failure\n");
}

void print_HELP_result(int ok) {
  if (ok) {
    printf("succes\n");
    return;
  }
  printf("failure\n");
}

