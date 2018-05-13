/* cklist.h - Custom list data structure for ck ------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2018  Anastasis Grammenos
 * GPLv3 (see LICENCE for the full notice)
 *
 * -------------------------------------------------------------------------- */

#include "cklist.h"
#include "ckutil.h"

cklist* list_make_new() {
  cklist *ckl = malloc(sizeof(cklist));
  ckl->size = 0;
  ckl->pos = 0;
  ckl->arr = NULL;

  return ckl;
}

void list_add(cklist *ckl, const char *str) {
  ckl->size++;
  ckl->arr = (char **)
    realloc(ckl->arr, ckl->size*sizeof(char *));

  if (!ckl->arr) {
    return;
  }

  ckl->arr[ckl->size-1] = strdup(str);
}

cklist* list_make_and_add(const char *str) {
  cklist *ckl = list_make_new();
  list_add(ckl,str);
  return ckl;
}

int list_next(cklist *ckl) {
  if (ckl->size < 1) {
    return 0;
  }
  if (ckl->pos >= ckl->size - 1) {
    return 0;
  }
  ckl->pos++;
  return 1;
}

char* list_get(cklist *ckl) {
  if (ckl->pos == -1) {
    return NULL;
  }
  if (ckl->pos >= ckl->size) {
    return NULL;
  }
  return ckl->arr[ckl->pos];
}

char* list_get_at(cklist *ckl, int pos) {
  if (ckl->pos == -1) {
    return NULL;
  }
  if (ckl->pos >= ckl->size
      || pos >= ckl->size) {
    return NULL;
  }
  return ckl->arr[pos];
}

void list_rewind(cklist *ckl) {
  ckl->pos = 0;
}

cklist* list_duplicate(cklist *ckl) {
  list_rewind(ckl);
  cklist *_ckl = list_make_and_add(list_get(ckl));
  while (list_next(ckl)) {
    list_add(_ckl, list_get(ckl));
  }
  list_rewind(ckl);
  return _ckl;
}

cklist* list_move(cklist *ckl) {
  list_rewind(ckl);
  cklist *_ckl = list_make_and_add(list_get(ckl));
  while (list_next(ckl)) {
    list_add(_ckl, list_get(ckl));
  }
  list_free(ckl);
  return _ckl;
}

cklist* list_copy_from(cklist *ckl, int index) {
  list_rewind(ckl);
  cklist *_ckl = list_make_new();
  if (ckl->pos >= index) {
    list_add(_ckl, list_get(ckl));
  }
  while(list_next(ckl)) {
    if (ckl->pos >= index) {
      list_add(_ckl, list_get(ckl));
    }
  }
  list_rewind(ckl);
  return _ckl;
}

cklist* list_copy_until(cklist *ckl, int index) {
  list_rewind(ckl);
  cklist *_ckl = list_make_and_add(list_get(ckl));
  while(list_next(ckl)) {
    if (ckl->pos < index) {
      list_add(_ckl, list_get(ckl));
    }
  }
  list_rewind(ckl);
  return _ckl;
}

cklist* list_copy_part(cklist *ckl, int from, int until) {
  list_rewind(ckl);
  cklist *_ckl = list_make_new();
  if (ckl->pos >= from && ckl->pos < until) {
    list_add(_ckl, list_get(ckl));
  }
  while(list_next(ckl)) {
    if (ckl->pos >= from && ckl->pos < until) {
      list_add(_ckl, list_get(ckl));
    }
  }
  list_rewind(ckl);
  return _ckl;
}

void list_print_lisp(cklist *ckl) {
  if (ckl->size > 0) {
    list_rewind(ckl);
    printf("( \"%s\"", list_get(ckl));
    if (ckl->size == 1) {
      printf(" )\n");
    }
    while (list_next(ckl)) {
      if (ckl->pos == ckl->size-1) {
        printf(", \"%s\" )\n", list_get(ckl));
      }
      else {
        printf(", \"%s\"", list_get(ckl));
      }
    }
    list_rewind(ckl);
  }
}

void list_print(cklist *ckl) {
  if (ckl->size > 0) {
    list_rewind(ckl);
    while (list_next(ckl)) {
      printf("%s\n", list_get(ckl));
    }
    list_rewind(ckl);
  }
}

int list_size(cklist *ckl) {
  return ckl->size;
}

void list_free(cklist *ckl) {
  int i;
  for (i=0; i<ckl->size; i++) {
    free(ckl->arr[i]);
  }
  free(ckl->arr);
  free(ckl);
}
