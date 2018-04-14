#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#include "actionparser.h"
#include "actions.h"
#include "confparser.h"
#include "dblayer.h"

 int main(int argc, char *argv[]) {
  sqlite3 *db;
  int rc;

  Conf conf;
  config_file_parse(&conf);
    //return 1;

#define X(var, str, name) \
  printf("%s: %s\n", name, conf.var);
  CONFIG_VARIABLES_TABLE
#undef X
  UserOpt opt;
  switch(parseAction(argc, argv, &opt)) {
  case OPR_HELP:
    printParserHelp();
  case OPR_ERR:
    printParserError(&opt);
  case OPR_OK:
    break;
    //
  }

  switch(opt.action) {
#define X(ACTION)                               \
    case CKA_##ACTION:                          \
      run_##ACTION(&opt, &conf);                \
      break;
    CK_ACTIONS
#undef X

  }

  return 0;
}
