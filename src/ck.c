/* ck.c - the main -----------------------------------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2018  Anastasis Grammenos
 * GPLv3 (see LICENCE for the full notice)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * -----------------------------------------------------------------------------
 *
 * This is the file where the main function lies.
 * It first parses the user options and config, then passes 
 * the results to the run_ACTION functions.
 *
 * -------------------------------------------------------------------------- */

#include "actions.h"
#include "dblayer.h"
#include "cklist.h"
#include "ckutil.h"

void free_res(UserOpt *opt, Conf *conf) {
  if (opt) {
    free_user_opt(opt);
  }
  if (conf) {
    free_conf(conf);
  }
}

int main(int argc, char *argv[]) {
  UserOpt opt;
  /* get user opt */
  switch(parse_action(argc, argv, &opt)) {
  case APR_HELP:
    free_res(&opt, NULL);
    print_parser_help();
    return 0;
  case APR_ERR:
    print_parser_error(&opt);
    free_res(&opt, NULL);
    return 1;
  case APR_OK:
    break;
  }

  Conf conf = {.VC_dir = NULL, .SCRT_dir = NULL};
  /* If the action is init don't load the config, skip to running init*/
  if (opt.action != CKA_INIT) {
    /* If the db doesn't exist ck is not initialized in the config 
     * location specified in opt */
    if (!db_exists(&opt)) {
      printf("ck is not initialized in %s.\nRun ck init first.\n", opt.confDir);
      free_res(&opt, NULL);
      return 1;
    }
    /* Finally parse the config file and exit on error */
    if (!config_file_parse(&conf, &opt)) {
      free_res(&opt, &conf);
      return 1;
    }
  }

  /* Run action and print results */
  switch(opt.action) {
#define X(ACTION)                                               \
    case CKA_##ACTION:                                          \
      print_##ACTION##_result(run_##ACTION(&opt, &conf));       \
      break;
    CK_ACTIONS
#undef X
  default:
    break;
  }
  free_res(&opt, &conf);
  return 0;
}
