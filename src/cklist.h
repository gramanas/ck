/* cklist.h - Custom list data structure for ck ------------------------*- C -*-
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
 * Ck needs a list of strings. Assists with finding the correct config file
 * to edit when using ck edit and provides a backend to the ck list action.
 *
 * -------------------------------------------------------------------------- */

typedef struct cklist_st cklist;
struct cklist_st {
  int size;
  int pos;
  char **arr;
};

extern cklist* list_make_new();
extern void list_add(cklist *ckl, const char *str);
extern cklist* list_make_and_add(const char *str);
extern void list_rewind(cklist *ckl);

extern int list_next(cklist *ckl);
extern char* list_get(cklist *ckl);

extern void list_size(cklist *ckl);

/* rewinds */
extern cklist* list_duplicate(cklist *ckl);
/* rewinds */
extern cklist* list_move(cklist *ckl);
/* rewinds 
 * copy from index (>=) to the end */
extern cklist* list_copy_from(cklist *ckl, int index);
/* rewinds 
 * copy from the start until (<) index*/
extern cklist* list_copy_until(cklist *ckl, int index);

/* rewinds 
 * copy from (>=) until (<) */
extern cklist* list_copy_part(cklist *ckl, int from, int until);

/* rewinds */
extern void list_print_lisp(cklist *ckl);
/* rewinds */
extern void list_print(cklist *ckl);

/* Deallocate resources */
extern void list_free(cklist *ckl);

