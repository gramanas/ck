/* ck.c - Main ck file -------------------------------------------------*- C -*-
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
 * It first parses the user options and then runs the
 * handles the list of user arguments to the run_ACTION functions.
 *
 * -------------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#include "actionparser.h"
#include "actions.h"
#include "confparser.h"
#include "dblayer.h"

int main(int argc, char *argv[]) {
  UserOpt opt;
  switch(parseAction(argc, argv, &opt)) {
  case OPR_HELP:
    free_user_opt(&opt);
    printParserHelp();
    return 0;
  case OPR_ERR:
    printParserError(&opt);
    free_user_opt(&opt);
    return 1;
  case OPR_OK:
    break;
  }

  Conf conf;
  if (opt.action != CKA_INIT) {
    if (!db_exists(&opt)) {
      printf("ck is not initialized in %s.\nRun ck init first.\n", opt.confDir);
      free_user_opt(&opt);
      return 1;
    }
    if (!config_file_parse(&conf, &opt)) {
      free_user_opt(&opt);
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
  }
  free_user_opt(&opt);
  return 0;
}
