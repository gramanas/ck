/* engine.c - manages the linking and copying of configs ---------------*- C -*-
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
 *
 *
 * -------------------------------------------------------------------------- */
#include "engine.h"

char engine_err[STR_M] = "";

int engine_err_message(char *err) {
  if (!str_is_empty(engine_err)) {
    if (err) {
      strcpy(err, engine_err);
    }
    return 1;
  }
  return 0;
}

int copy_config(char *path);


void link_config(const AddOpt *opt, const char* newPath) {
  printf("Linking %s -> %s\n", newPath, opt->confPath);
  if (util_symlink_file(newPath, opt->confPath) != 0) {
    strcpy(engine_err, "Could not link file.");
  }
}

void move_config(const AddOpt *opt, const Conf *conf, char *ret) {
  char newName[STR_M];
  str_make_ck_config_name(newName, opt->confPath, opt->progName);
  char newPath[STR_L];
  str_join_dirname_with_basename(newPath, opt->secret ? conf->SCRT_dir : conf->VC_dir, newName);
  printf("Moving %s -> %s\n", opt->confPath, newPath);
  strcpy(ret, newPath);
  if (util_move_file(opt->confPath, newPath) != 0) {
    strcpy(engine_err, "Could not move file.");
  }
}

void engine_add_make_link(const AddOpt *opt,
                          const Conf *conf) {
  char newPath[STR_L];
  move_config(opt, conf, newPath);
  if (engine_err_message(NULL)) {
    return;
  }
  link_config(opt, newPath);
  if (engine_err_message(NULL)) {
    return;
  }
}
