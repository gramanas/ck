/* actionparser.c - Action parser for ck -------------------------------*- C -*-
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
 * The following code is resposinble for parsing the command line arguments
 * and report any errors that might come up.
 *
 * -------------------------------------------------------------------------- */
#include "actionparser.h"
#include "ckutil.h"

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
  opt->argv[position] = (char *)malloc((strlen(token))*sizeof(char) + 1);
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
  return -1;
}
int parse_EDIT(UserOpt *opt) {
  return -1;
}
int parse_LIST(UserOpt *opt) {
  return -1;
}
int parse_SEARCH(UserOpt *opt) {
  return -1;
}
int parse_HELP(UserOpt *opt) {
  return -1;
}


int parseVals(UserOpt *opt) {
  switch (opt->action) {
#define X(ACTION)                                \
    case CKA_##ACTION:                           \
      return parse_##ACTION(opt);
    CK_ACTIONS
#undef X
  default:
    return -1;
  }
}

CkAction determineAction() {
  int i = 0;
#define X(ACTION)                                                       \
  for (i = 1; i < atoi(str##ACTION[0]) + 1; i++) {                      \
    if (strcmp(token, str##ACTION[i]) == 0) {                           \
      return CKA_##ACTION;                                              \
    }                                                                   \
  }
  CK_ACTIONS
#undef X
  return CK_WRONG_ACTION;
}

UserOpt make_empty_user_opt() {
  UserOpt opt;
  opt.action = -1;
  opt.err = PERR_NOERR;
  opt.argc = 0;
  opt.confDir = NULL;
  for (int i = 0; i < 10; i++) {
    opt.argv[i] = NULL;
  }
  return opt;
}

void free_user_opt(UserOpt *opt) {
  for (int i = 0; i < 10; i++) {
    if (opt->argv[i] != NULL) {
      free(opt->argv[i]);
    }    
  }
  if (opt->confDir != NULL) {
    free(opt->confDir);
  }
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
      if (strcmp(token, ".") == 0){
        printf("Dot\n");
      }
      if (!util_is_dir(token)) {
        printf("%s is not a directory\n", token);
        exit(1);
      }
      opt->confDir = malloc(strlen(token) + 1);
      strcpy(opt->confDir, token);
      // remove trailing `/`
      if (opt->confDir[strlen(token) - 1] == '/') {
        opt->confDir[strlen(token) - 1] = '\0';
      }
      return;
    }
  }
  char * defaultConf = "/.ck";
  char * home = getenv("HOME");
  opt->confDir = malloc(strlen(defaultConf)+strlen(home)+1);
  strcpy(opt->confDir, home);
  strcat(opt->confDir, defaultConf);

  // rewind
  pos = pos - 1;
  token = opts[pos];
}

ParseResult parseAction(int argc, char* argv[], UserOpt *opt) {
  *opt = make_empty_user_opt();
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
  if (opt->action == CK_WRONG_ACTION) {
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

void getPossibleActionNames(char * dest, CkAction ckAction) {
  if (ckAction == -1) {
    dest = NULL;
    return;
  }

  char buf[STR_S];
  
  switch (ckAction) {
#define X(ACTION)                                         \
    case CKA_##ACTION:                                    \
    strcpy(buf, "{ ");                                    \
    for (int i = 1; i < atoi(str##ACTION[0]); i++) {      \
      strcat(buf, str##ACTION[i]);                        \
      strcat(buf, ", ");                                  \
    }                                                     \
    strcat(buf, str##ACTION[atoi(str##ACTION[0])]);       \
    strcat(buf, " }");                                    \
    break;
    CK_ACTIONS
#undef X
  default:
    break;    
  }

  strcpy(dest, buf);
}

void printParserError(UserOpt *opt) {
  char errStr[STR_M];
  char names[STR_S];
  getPossibleActionNames(names, opt->action);

  switch (opt->err) {
  case PERR_NOERR:
    return;
  case PERR_UNKONW_ACTION:
    sprintf(errStr, "Unknown action: %s", token);
    break;
  case PERR_INIT_WRONG:
    sprintf(errStr, "Initialize database\nUsage: %s version_control_dir secret_dir", names);
    break;
  case PERR_ADD_WRONG:
    sprintf(errStr, "Add config (new or existing)\nUsage: %s ProgramName ConfigPath [-s](secret) [-p](primary)", names);
    break;
  case PERR_DEL_WRONG:
    sprintf(errStr, "Delete config or program\nUsage: %s ProgramName ConfigPath [-s](secret) [-p](primary)", names);
    break;
  case PERR_EDIT_WRONG:
    sprintf(errStr, "Edit config\nUsage: add ProgramName ConfigPath [-s](secret) [-p](primary)");
    break;
  case PERR_LIST_WRONG:
    sprintf(errStr, "Usage: .........)");
    break;
  case PERR_SEARCH_WRONG:
    sprintf(errStr, "Usage: .............");
    break;
  case PERR_HELP_WRONG:
    sprintf(errStr, "Usage: ........");
    break;
  }
  printf("Parsing error\n%s\n", errStr);
}

void printParserHelp() {
  char names[STR_S];
  printf("ck - the config keeper\n");
  printf("Usage:\n");
  getPossibleActionNames(names, CKA_INIT);
  printf("Initialize: \t%s\n",      names);
  getPossibleActionNames(names, CKA_ADD);
  printf("Add config: \t%s\n",       names);
  getPossibleActionNames(names, CKA_DEL);
  printf("Delete config: \t%s\n",    names);
  getPossibleActionNames(names, CKA_EDIT);
  printf("Edit config: \t%s\n",     names);
  getPossibleActionNames(names, CKA_LIST);
  printf("List configs: \t%s\n",    names);
  getPossibleActionNames(names, CKA_SEARCH);
  printf("Search: \t%s\n",        names);
  getPossibleActionNames(names, CKA_HELP);
  printf("Print this: \t%s\n",      names);
}
