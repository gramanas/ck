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
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <sys/sendfile.h>
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

void util_replace_slash_with_uscore(char *s) {
  int i = 0;
  while (*s != '\0') {
    if (*s == '/' && i != 0) {
      *s = '_';
    }
    s++;
    i++;
  }
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

int util_move_file(const char *path, const char* dest) {
  int srcFile = open(path, O_RDONLY);
  int destFile = open(dest, O_WRONLY | O_CREAT);
  struct stat st, newSt;

  fstat(srcFile, &st);
  sendfile(destFile, srcFile, NULL, st.st_size);
  close(srcFile);

  fchmod(destFile, st.st_mode);

  fstat(destFile, &newSt);
  if (st.st_size == newSt.st_size) {
    unlink(path);
    close(destFile);
    return 0;
  }
  close(destFile);
  return -1;
}

int util_symlink_file(const char *path, const char* dest) {
  return symlink(path, dest);
}

void str_make_ck_config_name(char *ret, const char *path,
                              const char *progName) {
  char *basec = strdup(path);
  char *bname = basename(basec);

  strcpy(ret, progName);
  strcat(ret, "_");
  strcat(ret, bname);
  free(basec);
}

void str_join_dirname_with_basename(char *ret, const char *dirname,
                                    const char *basename) {
  strcpy(ret, dirname);
  strcat(ret, "/");
  strcat(ret, basename);
}

int str_is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s)) {
      return 0;
    }
    s++;
  }
  return 1;
}
