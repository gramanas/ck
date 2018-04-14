#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include "confparser.h"

const char* const configFilename = ".ckrc";

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

char* read_next_line(FILE *f) {
  char nextLine[200];
  char *line;
  if (fgets(nextLine, 200, f) == NULL) {
    return NULL;
  }
  line = malloc(remove_newline(nextLine));
  strcpy(line, nextLine);
  return line;
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

ConfigParserResult parse(Conf *conf) {
  conf_values_initialize(conf);
  FILE *confPtr;
  if ((confPtr = fopen("/home/gramanas/.ck/ckrc", "r")) == NULL) {
    return CPR_NO_CONFIG_FILE;
  }
  int flag = 1;
  char *line = read_next_line(confPtr);
  char matched[200];
  while (line != NULL) {
    switch(match_variables(line, matched)) {
#define X(var, str, name)                       \
      case CV_##var:                            \
        conf->var = malloc(strlen(matched));    \
        strcpy(conf->var, matched);             \
        if (!is_dir(matched)) {                 \
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
    free(line);
    line=read_next_line(confPtr);
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

int config_file_parse(Conf *conf) {
  switch (parse(conf)) {
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
