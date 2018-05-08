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

#include "ckutil.h"
#include "cklist.h"
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
  char nextLine[STR_L];
  if (fgets(nextLine, STR_L, f) == NULL) {
    return -1;
  }
  strcpy(line, nextLine);
  return 0;
}

ConfVar match_variables(char *line, char matched[]) {
  if (line[0] == '#' || str_is_empty(line)) {
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

void make_config_name(char * ret, const char *confPath) {
  char tmp[STR_L];
  strcpy(tmp, confPath);
  strcat(tmp, CONFIG_NAME);

  strcpy(ret, tmp);
}

ConfigParserResult parse(Conf *conf, UserOpt *opt) {
  conf_values_initialize(conf);
  FILE *confPtr;
  char confName[STR_L];
  make_config_name(confName, opt->confDir);
  if ((confPtr = fopen(confName, "r")) == NULL) {
    return CPR_NO_CONFIG_FILE;
  }
  int flag = 1;
  char line[STR_L];
  char matched[STR_L];
  while (read_next_line(line, confPtr) == 0) {
    if (strlen(line) > STR_L) {
      return CPR_WRONG_CONFIG;
    }
    switch(match_variables(line, matched)) {
#define X(var, str, name)                       \
      case CV_##var:                            \
        conf->var = malloc(strlen(matched)+1);  \
        strcpy(conf->var, matched);             \
        if (!util_is_dir(matched)) {            \
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
      printf("--[ Config error ]--\n"                                   \
             "%s: %s\n"                                                 \
             "defined in config does not exist\n", name, conf->var);    \
      free(conf->var);                                                  \
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

void free_conf(Conf *conf) {
  if (conf->VC_dir) {
    free(conf->VC_dir);
  }
  if (conf->SCRT_dir) {
    free(conf->SCRT_dir);
  }
}

int init_create_config_file(UserOpt *opt) {
  list_rewind(opt->args);
  
  if (!util_file_exists(list_get(opt->args))) {
    printf("Version control directory: %s\ndoes not exist.\n", list_get(opt->args));
    return 1;
  }

  list_next(opt->args);
  if (!util_file_exists(list_get(opt->args))) {
    printf("Secret directory: %s\ndoes not exist.\n", list_get(opt->args));
    return 1;
  }
  
  if (!util_file_exists(opt->confDir)) {
    util_mkdir(opt->confDir);
  }

  char confName[STR_L];
  make_config_name(confName, opt->confDir);
  FILE *f;
  if ((f = fopen(confName, "w")) == NULL) {
    return 1;
  }

  list_rewind(opt->args);
  char tmp[200];
  strcpy(tmp, "version_control_dir = ");
  strcat(tmp, list_get(opt->args));
  strcat(tmp, "\n");
  fputs(tmp, f);

  list_next(opt->args);
  strcpy(tmp, "secret_dir = ");
  strcat(tmp, list_get(opt->args));
  strcat(tmp, "\n");
  fputs(tmp, f);

  fclose(f);
  list_rewind(opt->args);  
  return 0;
}
