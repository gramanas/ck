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
#include "dblayer.h"
#include "ckutil.h"

int run_INIT(UserOpt * opt, Conf *conf) {
  if (db_exists(opt)) {
    printf("Current configuration file location: %s\n", opt->confDir);
    PRINT_ERR("ck is already initialized.\n");
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

AddOpt make_add_options(const int argc, char **argv) {
  AddOpt addOpt = {
    .progName = argv[0],
    .confPath = NULL,
    .secret = 0,
    .prime = 0,
    .err = ADD_NO_ERR
  };
  
  /* the first two argumens have to exist since we are here */
  if (!util_is_file_rw(argv[1])) {
    addOpt.err = ADD_ERR_WRONG_CONFIG;
    return addOpt;
  }
  addOpt.confPath = argv[1];

  if (argc == 3) {
    if (strcmp(argv[2], "-s") == 0) {
      addOpt.secret = 1;
    } else if (strcmp(argv[2], "-p") == 0) {
      addOpt.prime = 1;
    } else {
      addOpt.err = ADD_ERR_WRONG_FLAGS;
      return addOpt;
    }
  } else if (argc == 4) {
    if (strcmp(argv[2], "-s") == 0) {
      addOpt.secret = 1;
    } else if (strcmp(argv[2], "-p") == 0) {
      addOpt.prime = 1;
    } else {
      addOpt.err = ADD_ERR_WRONG_FLAGS;
      return addOpt;
    }
    if (strcmp(argv[3], "-s") == 0) {
      addOpt.secret = 1;
    } else if (strcmp(argv[3], "-p") == 0) {
      addOpt.prime = 1;
    } else {
      addOpt.err = ADD_ERR_WRONG_FLAGS;
      return addOpt;
    }
  }
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
      PRINT_ERR("The database file is currupted. Run ck init anew.\n");
    }
  }
  AddOpt addOpt = make_add_options(opt->argc, opt->argv);
  switch (addOpt.err) {
  case ADD_NO_ERR:
    break;
  case ADD_ERR_WRONG_CONFIG:
    PRINT_ERR("The config file specified doesn't exist.\n");
    close_DB(&db);
    return 0;
  case ADD_ERR_WRONG_FLAGS:
    PRINT_ERR("Flags are: -s for secret and -p for primary.\n");
    close_DB(&db);
    return 0;
  }
  add_print_opts(&addOpt);
  if (add_transaction_begin(&db, addOpt.progName,
                            addOpt.confPath, addOpt.secret, addOpt.prime) == 0) {
    return 0;
  }
  // do the linking
  close_DB(&db);
  return 1;
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
  DB db = open_DB(opt);
  if (db.ptr == NULL) {
    if (db.error == SQL_ERR_NO_TABLES) {
      printf("no tables\n");
    }
  }
  for (int i = 0; i < opt->argc; i++) {
    printf("[%d]: %s\n", i, opt->argv[i]);
  }
  close_DB(&db);
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

