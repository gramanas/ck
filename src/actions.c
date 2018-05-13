/* actions.c - ck actions ----------------------------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2018  Anastasis Grammenos
 * GPLv3 (see LICENCE for the full notice)
 *
 * -------------------------------------------------------------------------- */
#include "actions.h"
#include "actionhelper.h"
#include "dblayer.h"
#include "ckutil.h"
#include "cklist.h"

int run_INIT(UserOpt * opt, Conf *conf) {
  UNUSED(conf);
  if (db_exists(opt)) {
    printf("Current configuration file location: %s\n", opt->confDir);
    PRINT_ERR("ck is already initialized.");
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

AddOpt make_add_options(cklist* args) {
  list_rewind(args);
  /* since we are here, the first two argumens must exist */
  AddOpt addOpt = {
    .progName = list_get(args),
    .confPath = NULL,
    .secret = 0,
    .prime = 0,
    .err = ADD_NO_ERR
  };

  list_next(args);
  if (!util_is_file_rw(list_get(args))) {
    addOpt.err = ADD_ERR_WRONG_CONFIG;
    return addOpt;
  }
  addOpt.confPath = list_get(args);

  while (list_next(args)) {
    if (strcmp(list_get(args), "-s") == 0 && addOpt.secret == 0) {
      addOpt.secret = 1;
    } else if (strcmp(list_get(args), "-p") == 0 && addOpt.prime == 0) {
      addOpt.prime = 1;
    } else {
      addOpt.err = ADD_ERR_WRONG_FLAGS;
      return addOpt;
    }
  }
  list_rewind(args);
  return addOpt;
}

void add_print_opts(AddOpt *opt) {
  printf("Program:\t%s\nConfig:\t\t%s\n", opt->progName, opt->confPath);
  if (opt->prime && opt->secret) {
    printf("Options:\tsecret, primary\n");
  } else if (opt->prime) {
    printf("Options:\tprimary\n");
  } else if (opt->secret) {
    printf("Options:\tsecret\n");
  }
}

int run_ADD(UserOpt * opt, Conf *conf) {
  DB db = open_DB(opt);
  if (db.ptr == NULL) {
    if (db.error == SQL_ERR_NO_TABLES) {
      PRINT_ERR("The database file is currupted. Run ck init anew.");
    }
    return 0;
  }
  AddOpt addOpt = make_add_options(opt->args);
  switch (addOpt.err) {
  case ADD_NO_ERR:
    break;
  case ADD_ERR_WRONG_CONFIG:
    PRINT_ERR("The config file specified doesn't exist.");
    close_DB(&db);
    return 0;
  case ADD_ERR_WRONG_FLAGS:
    PRINT_ERR("Flags are: -s for secret and -p for primary.");
    close_DB(&db);
    return 0;
  }
  add_print_opts(&addOpt);
  if (add_transaction_begin(&db, &addOpt) == 0) {
    close_DB(&db);
    return 0;
  }
  close_DB(&db);
  add_make_link(&addOpt, conf);
  char err[STR_M];
  if (add_err_message(err)) {
    PRINT_ERR(err);
    return 0;
  }
  return 1;
}

int run_DEL(UserOpt * opt, Conf *conf) {
  printf("Running %s\n", "del");
  return 0;
}

int run_EDIT(UserOpt *opt, Conf *conf) {
  printf("Running %s\n", "edit");
  DB db = open_DB(opt);
  if (db.ptr == NULL) {
    if (db.error == SQL_ERR_NO_TABLES) {
      PRINT_ERR("The database file is currupted. Run ck init anew.");
    }
    return 0;
  }

  list_rewind(opt->args);
  char confPath[STR_M];
  if (list_size(opt->args) == 1) {
    char confName[STR_M];
    int secret = 0;
    if (edit_get_prime_config_from_program(&db, list_get(opt->args), confName, &secret) == 1) {
      str_join_dirname_with_basename(confPath, secret ? conf->SCRT_dir : conf->VC_dir, confName);
      printf("%s\n", confPath);
    } else {
      PRINT_ERR("No primary config");
      close_DB(&db);
      return 0;
    }
  } else {
    close_DB(&db);
    char confName[STR_L];
    switch (edit_get_config_or_suggestions(opt->args, confName)) {
    case ERC_OK:
      return 0;
    case ERC_ERR:
      return 1;
    case ERC_SUGGESTIONS:
      return 1;
    }
    return 1;
  }
  close_DB(&db);
    
  char *editor = getenv("EDITOR");
  char command[STR_L];
  strcpy(command, str_is_empty(editor) ? "nano" : editor);
  strcat(command, " ");
  strcat(command, confPath);
  system(command);
  return 1;
}

int run_LIST(UserOpt *opt, Conf *conf) {
  printf("Running %s\n", "list");
  DB db = open_DB(opt);
  if (db.ptr == NULL) {
    if (db.error == SQL_ERR_NO_TABLES) {
      printf("no tables\n");
    }
    return 0;
  }
  list_print_lisp(opt->args);
  close_DB(&db);
  return 0;
}

int run_SEARCH(UserOpt *opt, Conf *conf) {
  printf("Running %s\n", "search");
  return 0;
}

int run_HELP(UserOpt *opt, Conf *conf) {
  printf("Running %s\n", "help");
  return 0;
}

void print_INIT_result(int ok) {
  if (ok) {
    printf("Initialized empty ckdb.\n");
  }
}

void print_ADD_result(int ok) {
  if (ok) {
    printf("ckdb updated succesfully.\n");
    return;
  }
  printf("Could not complete add transaction.\n");
}

void print_DEL_result(int ok) {
  if (ok) {
    printf("succes\n");
    return;
  }
  printf("Not Supported\n");
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
  printf("Not Supported\n");
}

void print_SEARCH_result(int ok) {
  if (ok) {
    printf("succes\n");
    return;
  }
  printf("Not Supported\n");
}

void print_HELP_result(int ok) {
  if (ok) {
    printf("succes\n");
    return;
  }
  printf("failure\n");
}

