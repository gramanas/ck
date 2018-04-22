/* ckutil.h - utility functions for ck ---------------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2018  Anastasis Grammenos
 * GPLv3 (see LICENCE for the full notice)
 *
 * -------------------------------------------------------------------------- */
#ifndef CKUTIL_H
#define CKUTIL_H

/* Returns 1 if path is a directory, else returns 0. */
extern int util_is_dir(const char *path);

/* Returns 1 if file(or dir) exists, else returns 0. */
extern int util_file_exists(const char *path);

extern int util_is_file_rw(const char *path);
extern int util_is_cli_flag();

/* Returns 1 if str contains only whitespace, or nothing, 
 * else returns 0. */
extern int util_is_str_empty();
extern int util_remove_newlibe();

extern void util_mkdir(const char *path);
#endif // CKUTIL_H
