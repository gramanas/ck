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
