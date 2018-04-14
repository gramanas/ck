#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "actions.h"
#include "dblayer.h"

int init_create_config_file(UserOpt *opt) {
  struct stat st = {0};
  if (stat("/home/gramanas/.ck", &st)  == -1) {
    mkdir("/home/gramanas/.ck", 0755);
  }
  
  FILE *f;
  if ((f = fopen("/home/gramanas/.ck/ckrc", "w")) == NULL) {
    return 1;
  }
  char tmp[200];
  if (stat(opt->argv[0], &st) == -1) {
    printf("Version control directory: %s\ndoes not exist.\n", opt->argv[0]);
    return 1;
  }
  strcpy(tmp, "version_control_dir = ");
  strcat(tmp, opt->argv[0]);
  strcat(tmp, "\n");
  fputs(tmp, f);
  if (stat(opt->argv[1], &st) == -1) {
    printf("Secret directory: %s\ndoes not exist.\n", opt->argv[1]);
    return 1;
  }
  strcpy(tmp, "secret_dir = ");
  strcat(tmp, opt->argv[1]);
  strcat(tmp, "\n");
  fputs(tmp, f);
  fclose(f);
  return 0;
}

int run_INIT(UserOpt * opt, Conf *conf) {
  if (db_exists()) {
    printf("ck is already initialized.\n");
    return 0;
  }
  if (init_create_config_file(opt)) {
    return 0;
  }
  DB db = init_make_DB();
  if (db.error == SQL_NO_ERR) {
    init_make_tables(&db);
  }
  sqlite3_close(db.get);
  return 1;
}

int run_ADD(UserOpt * opt, Conf *conf) {
  if (!db_exists()) {
    printf("ck is not initialized.\nRun ck init first.");
    return 0;
  }
  printf("Running %s\n", "add");
  DB db = open_DB();
  if (db.get == NULL) {
    if (db.error == SQL_ERR_NO_TABLES) {
      printf("no tables\n");
    }
  }
  return 0;
}

int run_DEL(UserOpt * opt, Conf *conf) {
  if (!db_exists()) {
    printf("ck is not initialized.\nRun ck init first.");
    return 0;
  }
  printf("Running %s\n", "del");
  return 0;
}

int run_EDIT(UserOpt * opt, Conf *conf) {
  if (!db_exists()) {
    printf("ck is not initialized.\nRun ck init first.");
    return 0;
  }
  printf("Running %s\n", "edit");
  return 0;
}

int run_LIST(UserOpt * opt, Conf *conf) {
  if (!db_exists()) {
    printf("ck is not initialized.\nRun ck init first.");
    return 0;
  }
  printf("Running %s\n", "list");
  return 0;
}

int run_SEARCH(UserOpt * opt, Conf *conf) {
  if (!db_exists()) {
    printf("ck is not initialized.\nRun ck init first.");
    return 0;
  }
  printf("Running %s\n", "search");
  return 0;
}

int run_HELP(UserOpt * opt, Conf *conf) {
  if (!db_exists()) {
    printf("ck is not initialized.\nRun ck init first.");
    return 0;
  }
  printf("Running %s\n", "help");
  return 0;
}
