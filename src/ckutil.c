/* ckutil.c - utility functions for ck ---------------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2018  Anastasis Grammenos
 * GPLv3 (see LICENCE for the full notice)
 *
 * -------------------------------------------------------------------------- */
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ckutil.h"

int util_is_dir(const char *path) {
  DIR *dir;
  dir = opendir(path);
  if (!dir) {
    return 0;
  }
  closedir(dir);
  return 1;
}

int util_is_str_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}

int util_file_exists(const char* path) {
  struct stat st = {0};
  if (stat(path, &st) == -1) {
    return 0;
  }
  return 1;
}

int util_is_file_rw(const char* path) {
  if (access(path, R_OK | W_OK) == 0) {
    return 1;
  }
  return 0;
}

void util_mkdir(const char *name) {
  mkdir(name, 0755);
}
