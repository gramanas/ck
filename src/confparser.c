/* confparser.h - Configuration file parser for ck ---------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2018  Anastasis Grammenos
 * GPLv3 (see LICENCE for the full notice)
 *
 * -------------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "confparser.h"


const char * const CONFIG_NAME = "/ckrc";

void conf_values_initialize(Conf *c) {
  c->SCRT_dir = NULL;
  c->VC_dir = NULL;
}

int remove_newline(char buff[]) {
  int initLength = strlen(buff);
  for (int i = 0; i < initLength; i++) {
    if (buff[i] == '\n') {
      buff[i] = ' ';
    }
  }
  return strlen(buff);
}

int read_next_line(char *line, FILE *f) {
  char nextLine[200];
  if (fgets(nextLine, 200, f) == NULL) {
    return -1;
  }
  strcpy(line, nextLine);
  return 0;
}

int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}

ConfVar match_variables(char *line, char matched[]) {
  if (line[0] == '#' || is_empty(line)) {
    return CV_NO_VAL_OR_COMMENT;
  }
#define X(var, str, name)                        \
  if (sscanf(line, str, matched) == 1) {         \
    return CV_##var;                             \
  }
  CONFIG_VARIABLES_TABLE
#undef X
  return -1;
}

int is_dir(char *path) {
  DIR *dir;
  dir = opendir(path);
  if (!dir) {
    return 0;
  }
  closedir(dir);
  return 1;
}

char *make_config_name(char * confPath) {
  char *db_path = strdup(confPath);

  db_path = realloc(db_path, strlen(confPath) + strlen(CONFIG_NAME)+1);
  strcat(db_path, CONFIG_NAME);

  return db_path;
}

ConfigParserResult parse(Conf *conf, UserOpt *opt) {
  conf_values_initialize(conf);
  FILE *confPtr;
  char *confName = make_config_name(opt->confDir);
  if ((confPtr = fopen(confName, "r")) == NULL) {
    free(confName);
    return CPR_NO_CONFIG_FILE;
  }
  free(confName);
  int flag = 1;
  char line[200];
  char matched[200];
  while (read_next_line(line, confPtr)) {
    switch(match_variables(line, matched)) {
#define X(var, str, name)                       \
      case CV_##var:                            \
        conf->var = malloc(strlen(matched)+1);  \
        strcpy(conf->var, matched);             \
        if (!is_dir(matched)) {                 \
          free(conf->var);                      \
          return CPR_WRONG_##var;               \
        }                                       \
        break;
      CONFIG_VARIABLES_TABLE
#undef X
    case CV_NO_VAL_OR_COMMENT:
      break;
    default:
      printf("%s:\n%s\n", "Config error in line", line);
    }
  }
#define X(var, str)                               \
  if (conf->var == NULL) {                        \
    flag = 0;                                     \
    CONFIG_VARIABLES_TABLE
#undef X
  if (flag) {
    return CPR_OK;
  }
  return CPR_NO_CONFIG_FILE;
}

int config_file_parse(Conf *conf, UserOpt *opt) {
  switch (parse(conf, opt)) {
#define X(var,str,name)                                                 \
    case CPR_WRONG_##var:                                               \
      printf("Config error:\n"                                          \
             "%s: %s\n"                                                 \
             "defined in config does not exist\n", name, conf->var);    \
      return 0;                                                         \
    break;
    CONFIG_VARIABLES_TABLE
#undef X
  case CPR_NO_CONFIG_FILE:
    printf("The config file specified could not be found\n");
    return 0;
  case CPR_WRONG_CONFIG:
    printf("Config help\n");
  case CPR_OK:
    return 1;
  }
}

int init_create_config_file(UserOpt *opt) {
  struct stat st = {0};
  char tmp[200];
  if (stat(opt->argv[0], &st) == -1) {
    printf("Version control directory: %s\ndoes not exist.\n", opt->argv[0]);
    return 1;
  }

  if (stat(opt->argv[1], &st) == -1) {
    printf("Secret directory: %s\ndoes not exist.\n", opt->argv[1]);
    return 1;
  }
  
  if (stat(opt->confDir, &st)  == -1) {
    mkdir(opt->confDir, 0755);
  }

  char *confName = make_config_name(opt->confDir);
  FILE *f;
  if ((f = fopen(confName, "w")) == NULL) {
    free(confName);
    return 1;
  }
  
  strcpy(tmp, "version_control_dir = ");
  strcat(tmp, opt->argv[0]);
  strcat(tmp, "\n");
  fputs(tmp, f);

  strcpy(tmp, "secret_dir = ");
  strcat(tmp, opt->argv[1]);
  strcat(tmp, "\n");
  fputs(tmp, f);
  fclose(f);

  free(confName);
  return 0;
}
