#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "actionparser.h"

/* --------------------------------------------------------------------------
 * The following code is resposinble for parsing the command line arguments
 * and report any errors that might come up.
 * -------------------------------------------------------------------------- */

/* accepted commands */
/* [0] is the count */
const char* const strINIT[] = {"2", "init", "-i"};
const char* const strADD[] = {"2", "add", "-a"};
const char* const strDEL[] = {"2", "del", "-d"};
const char* const strEDIT[] = {"3", "edit", "e", "-e"};
const char* const strLIST[] = {"3", "list", "ls", "-ls"};
const char* const strSEARCH[] = {"3", "search", "s", "-s"};
const char* const strHELP[] = {"4", "help", "?", "-h", "--help"};
const char* const strConfDir[] = {"3", "config", "conf", "-c"};

/* Number of opts */
static int optNum;
/* holds the list of the opts
   as given by the user */
static char **opts;
/* points to the current token */
static char *token;
/* points to the position
 * to be read */
static int pos = 0;

/* Reads the next token and returns it's position
 * Returns -1 otherwise */
int nextToken() {
  if (pos < optNum) {
    token = opts[pos];
    return pos++;
  }
  else {
    token = "";
    return -1;
  }
}

void getOpt(int position, UserOpt *opt) {
  // get arg
  nextToken();

  // allocate memory
  opt->argv[position] = (char *)malloc((strlen(token)+1)*sizeof(char));
  strcpy(opt->argv[position], token);
}

/* When starting to parse the action,
 * `pos` should be at 2
 * like so "ck ACTION ..."
 *                    ^               */
int parse_INIT(UserOpt *opt) {
  // INIT expects 2 arguments
  // starting from 0
  opt->argc = 2;
  if (optNum != pos /* already consumed */ + opt->argc) {
    opt->err = PERR_INIT_WRONG;
    return -1;
  }

  for (int i = 0; i < opt->argc; i++) {
    getOpt(i, opt);
  }
  return 1;
}

int parse_ADD(UserOpt *opt) {
  // ADD expects 2 to 4 arguments
  if (optNum <= pos + 1
      || optNum > pos + 4) {
    opt->err = PERR_ADD_WRONG;
    return -1;
  }

  opt->argc = optNum - pos;
  for (int i = 0; i < opt->argc; i++) {
    getOpt(i, opt);
  }
  return 1;
}

int parse_DEL(UserOpt *opt) {
}
int parse_EDIT(UserOpt *opt) {
}
int parse_LIST(UserOpt *opt) {
}
int parse_SEARCH(UserOpt *opt) {
}
int parse_HELP(UserOpt *opt) {
}


int parseVals(UserOpt *opt) {
  switch (opt->action) {
#define X(ACTION)                             \
    case CKA_##ACTION:                           \
      return parse_##ACTION(opt);
    CK_ACTIONS
#undef X
  default:
    // can't end up here, but it prevents a compiler warning
    opt->err = PERR_UNKONW_ACTION;
    return -1;
    break;
  }
  return 1;
}

CkAction determineAction() {
  int i = 0;
#define X(ACTION)                                                    \
  for (i = 1; i < atoi(str##ACTION[0]) + 1; i++) {                      \
    if (strcmp(token, str##ACTION[i]) == 0) {                           \
      return CKA_##ACTION;                                              \
    }                                                                   \
  }
  CK_ACTIONS
#undef X
  return -1;
}

UserOpt initUserOpt() {
  UserOpt uo;
  uo.action = -1;
  uo.err = PERR_NOERR;
  uo.argc = 0;
  for (int i = 0; i < 10; i++) {
    uo.argv[i] = NULL;
  }
  return uo;
}


void getConfig(UserOpt *opt) {
  // get first token
  nextToken();
  int ok = 1;
  for (int i = 1; i < atoi(strConfDir[0]) + 1; i++) {
    if (strcmp(token, strConfDir[i]) == 0) {
      if (nextToken() == -1) {
        printf("Config needs a value\n");
        exit(1);
      }
      struct stat st = {0};
      if (stat(token, &st) == -1) {
        printf("%s is not a directory\n", token);
        exit(1);
      }
      opt->confDir = malloc(sizeof(token));
      strcpy(opt->confDir, token);
      return;
    }
  }
  char * defaultConf = "/.ck";
  char * home = getenv("HOME");
  opt->confDir = malloc(sizeof(defaultConf)+sizeof(home));
  strcpy(opt->confDir, home);
  strcat(opt->confDir, defaultConf);

  // rewind
  pos = pos - 1;
  token = opts[pos];
}

ParseResult parseAction(int argc, char* argv[], UserOpt *opt) {
  *opt = initUserOpt();
  if (argc < 2) {
    return OPR_HELP;
  }
  opts = argv;
  optNum = argc;

  // skip the program nake
  nextToken();

  getConfig(opt);
  // get action
  nextToken();
  opt->action = determineAction();
  if (opt->action == -1) {
    opt->err = PERR_UNKONW_ACTION;
    return OPR_ERR;
  }
  if (opt->action == CKA_HELP) {
    return OPR_HELP;
  }

  // parse values
  if (!parseVals(opt)) {
    return OPR_ERR;
  }

  if (opt->err == PERR_NOERR) {
    return OPR_OK;
  }
  else {
    return OPR_ERR;
  }
}

const char * getPossibleActionName(const char* const strAction[]) {
  char *names;
  size_t size = 2; // first chars "{ "
  for (int i = 1; i < atoi(strAction[0]) + 1; i++) {
    size += strlen(strAction[i]) + 2; // comma and space for each entry and " }" for the last one
  }
  if ((names = malloc(size)) != NULL) {
    strcpy(names, "{ ");
    int i = 1;
    for (; i < atoi(strAction[0]); i++) {
      strcat(names, strAction[i]);
      strcat(names, ", ");
    }
    // last one
    strcat(names, strAction[atoi(strAction[0])]);
    strcat(names, " }");
    return names;
  }
  return NULL;
}

void printParserError(UserOpt *opt) {
  char *errStr = NULL;
  switch (opt->err) {
  case PERR_NOERR:
    return;
  case PERR_UNKONW_ACTION:
    asprintf(&errStr, "Unknown action: %s", token);
    break;
  case PERR_INIT_WRONG:
    asprintf(&errStr, "Initialize database\nUsage: %s version_control_dir secret_dir", getPossibleActionName(strINIT));
    break;
  case PERR_ADD_WRONG:
    asprintf(&errStr, "Add config (new or existing)\nUsage: %s ProgramName ConfigPath [-s](secret) [-p](primary)", getPossibleActionName(strADD));
    break;
  case PERR_DEL_WRONG:
    asprintf(&errStr, "Delete config or program\nUsage: %s ProgramName ConfigPath [-s](secret) [-p](primary)", getPossibleActionName(strDEL));
    break;
  case PERR_EDIT_WRONG:
    asprintf(&errStr, "Edit config\nUsage: add ProgramName ConfigPath [-s](secret) [-p](primary)");
    break;
  case PERR_LIST_WRONG:
    asprintf(&errStr, "Usage: add ProgramName ConfigPath [-s](secret) [-p](primary)");
    break;
  case PERR_SEARCH_WRONG:
    asprintf(&errStr, "Usage: add ProgramName ConfigPath [-s](secret) [-p](primary)");
    break;
  case PERR_HELP_WRONG:
    asprintf(&errStr, "Usage: add ProgramName ConfigPath [-s](secret) [-p](primary)");
    break;
  }
  printf("Parsing error\n%s\n", errStr);
  exit(1);
}

void printParserHelp() {
  printf("ck - the config keeper\n"                               );
  printf("Usage:\n"                                               );
  printf("Initialize: \t%s\n",      getPossibleActionName(strINIT));
  printf("Add config: \t%s\n",       getPossibleActionName(strADD));
  printf("Delete config: \t%s\n",    getPossibleActionName(strDEL));
  printf("Edit config: \t%s\n",     getPossibleActionName(strEDIT));
  printf("List configs: \t%s\n",    getPossibleActionName(strLIST));
  printf("Search: \t%s\n",        getPossibleActionName(strSEARCH));
  printf("Print this: \t%s\n",      getPossibleActionName(strHELP));
  exit(0);
}
