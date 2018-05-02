/* ckutil.h - utility functions for ck ---------------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2018  Anastasis Grammenos
 * GPLv3 (see LICENCE for the full notice)
 *
 * -----------------------------------------------------------------------------
 *
 * Utility funtions and global constants.
 *
 * -------------------------------------------------------------------------- */
#ifndef CKUTIL_H
#define CKUTIL_H

/*******************/
/* global includes */
/*******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT_ERR(ERROR)                             \
  printf("--[ Error ]--\n%s\n", ERROR);

#define UNUSED(x) (void)(x)

/********************/
/* global constants */
/********************/

/* String length - Small */
#define STR_S 30

/* String length -Medium */
#define STR_M 100

/* String length - Large */
#define STR_L 400

/********************/
/* string functions */
/********************/

/* Create the config name to be used when storing a new config to 
 * the VC or SCRT dir */
extern void str_make_ck_config_name(char *ret, const char *path,
                                     const char *progName);

/* Joins the two strings into ret, with a '/' in between */
extern void str_join_dirname_with_basename(char *ret, const char *path,
                                           const char *progName);

/* Returns 1 if str contains only whitespace, or nothing, 
 * else returns 0. */
extern int str_is_empty(const char *s);

/*********************/
/* utility functions */
/*********************/

/* Returns 1 if path is a directory, else returns 0. */
extern int util_is_dir(const char *path);

/* Returns 1 if file(or dir) exists, else returns 0. */
extern int util_file_exists(const char *path);

/* Returns 1 if file(or dir) is readable and writable,
 * else returns 0. */
extern int util_is_file_rw(const char *path);

extern void util_replace_slash_with_uscore(char *s);

/* Wrapper around mkdir with 0775 permissions */
extern void util_mkdir(const char *path);

/* Moves file from path, to destination
 * keeping the same permissions.
 * Only deletes original on successful copy */
extern int util_move_file(const char *path, const char* dest);

/* Wrapper around symlink() */
extern int util_symlink_file(const char *path, const char* dest);
#endif // CKUTIL_H
