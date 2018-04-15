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
    printParserHelp();
  case OPR_ERR:
    printParserError(&opt);
  case OPR_OK:
    break;
  }

  printf("%s\n", opt.confDir);
  Conf conf;
  if (opt.action != CKA_INIT) {
    if (!db_exists()) {
      printf("ck is not initialized.\nRun ck init first.\n");
      return 1;
    }
    if (!config_file_parse(&conf)) {
      return 1;
    }
  }

  int ok;
  switch(opt.action) {
#define X(ACTION)                               \
    case CKA_##ACTION:                          \
      ok = run_##ACTION(&opt, &conf);           \
      break;
    CK_ACTIONS
#undef X
  }

  switch(opt.action) {
#define X(ACTION)                                       \
    case CKA_##ACTION:                                  \
      print_##ACTION##_result(ok);                      \
      break;
    CK_ACTIONS
#undef X    
  }
  return 0;
}
