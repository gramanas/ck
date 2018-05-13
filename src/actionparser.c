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
const char* const strINIT[]    = {"3", "init",   "i",  "-i"};
const char* const strADD[]     = {"3", "add",    "a",  "-a"};
const char* const strDEL[]     = {"3", "del",    "d",  "-d"};
const char* const strEDIT[]    = {"3", "edit",   "e",  "-e"};
const char* const strLIST[]    = {"5", "list",   "l",  "ls", "-l", "-ls"};
const char* const strSEARCH[]  = {"3", "search", "s",  "-s"};
const char* const strHELP[]    = {"5", "help",   "h",  "-?", "-h", "--help"};
const char* const strConfDir[] = {"4", "config", "conf", "c", "-c"};

/* Number of opts */
static int optNum;

/* holds the list of the opts
 * as given by the user */
static char **opts;

/* points to the current token */
static char *token;

/* the position to be read */
static int pos = 0;

/* Reads the next token and returns it's position
 * Returns -1 otherwise */
int next_token() {
  if (pos < optNum) {
    token = opts[pos];
    return pos++;
  }
  else {
    token = NULL;
    return -1;
  }
}

/* copy the option from the list
 * to the UserOpt struct */
void get_opt(UserOpt *opt) {
  /* get arg */
  next_token();
  list_add(opt->args, token);
}

void fill_args_list(int arg_num, UserOpt *opt) {
  for (int i = 0; i < arg_num; i++) {
    get_opt(opt);
  }
}

/* When starting to parse the action,
 * `pos` should be at 2
 * like so "ck ACTION ..."
 *                    ^               */
int parse_INIT(UserOpt *opt) {
  /* INIT expects 2 arguments
   * starting from 0 */
  int arg_num = 2;
  if (optNum != pos /* already consumed */ + arg_num) {
    opt->err = PERR_INIT_WRONG;
    return -1;
  }

  fill_args_list(arg_num, opt);
  return 1;
}

int parse_ADD(UserOpt *opt) {
  /* ADD expects 2 to 4 arguments */
  if (optNum <= pos + 1
      || optNum > pos + 4) {
    opt->err = PERR_ADD_WRONG;
    return -1;
  }

  int arg_num = optNum - pos;
  fill_args_list(arg_num, opt);
  return 1;
}

int parse_DEL(UserOpt *opt) {
  return -1;
}
int parse_EDIT(UserOpt *opt) {
  /* EDIT expects 1 to 2 arguments */
  if (optNum <= pos || optNum > pos + 2) {
    opt->err = PERR_EDIT_WRONG;
    return -1;
  }

  int arg_num = optNum - pos;
  fill_args_list(arg_num, opt);
  return 1;
}
int parse_LIST(UserOpt *opt) {
  /* List expects a maximum of than 2 arguments */
  if (optNum > pos + 2) {
    opt->err = PERR_EDIT_WRONG;
    return -1;
  }

  int arg_num = optNum - pos;
  fill_args_list(arg_num, opt);
  return 1;
}
int parse_SEARCH(UserOpt *opt) {
  return -1;
}
int parse_HELP(UserOpt *opt) {
  return -1;
}


int parse_vals(UserOpt *opt) {
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

void determine_action(UserOpt *opt) {
  /* get action */
  if (next_token() == -1) {
    opt->action = CK_WRONG_ACTION;
    return;
  }

  int i;
#define X(ACTION)                                                       \
  for (i = 1; i < atoi(str##ACTION[0]) + 1; i++) {                      \
    if (strcmp(token, str##ACTION[i]) == 0) {                           \
      opt->action = CKA_##ACTION;                                       \
      return;                                                           \
    }                                                                   \
  }
  CK_ACTIONS;
#undef X

  opt->action = CK_WRONG_ACTION;
  return;
}

UserOpt make_empty_user_opt() {
  UserOpt opt;
  opt.action = -1;
  opt.err = PERR_NOERR;
  opt.confDir = NULL;
  opt.args = list_make_new();
  return opt;
}


/* called to free the resources
 * UserOpt holds */
void free_user_opt(UserOpt *opt) {
  if (opt->confDir != NULL) {
    free(opt->confDir);
  }
  list_free(opt->args);
}

/* If the used has specified a config file other
 * than the default get it now */
void get_config(UserOpt *opt) {
  /* get first token */
  next_token();

  for (int i = 1; i < atoi(strConfDir[0]) + 1; i++) {
    if (strcmp(token, strConfDir[i]) == 0) {
      if (next_token() == -1) {
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
  char * defaultConf = ".ck";
  char * home = getenv("HOME");
  opt->confDir = malloc(strlen(defaultConf) + 1 /* '/' */ + strlen(home)+1);
  str_join_dirname_with_basename(opt->confDir, home, defaultConf);

  // rewind
  pos = pos - 1;
  token = opts[pos];
}

void get_possible_action_strings(char *dest, CkAction ckAction) {
  char buf[STR_S];
  switch (ckAction) {
#define X(ACTION)                                         \
    case CKA_##ACTION:                                    \
      strcpy(buf, "{ ");                                  \
      for (int i = 1; i < atoi(str##ACTION[0]); i++) {    \
        strcat(buf, str##ACTION[i]);                      \
        strcat(buf, ", ");                                \
      }                                                   \
      strcat(buf, str##ACTION[atoi(str##ACTION[0])]);     \
      strcat(buf, " }");                                  \
      break;
    CK_ACTIONS
#undef X
  default:
    dest = NULL;
    return;
  }

  strcpy(dest, buf);
}

void print_parser_error(UserOpt *opt) {
  char errStr[STR_L];
  char names[STR_S];
  get_possible_action_strings(names, opt->action);

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
    sprintf(errStr, "Add config \nUsage: %s ProgramName ConfigPath [-s](secret) [-p](primary)", names);
    break;
  case PERR_DEL_WRONG:
    sprintf(errStr, "Delete config or program\nUsage: %s ProgramName ConfigPath [-s](secret) [-p](primary)", names);
    break;
  case PERR_EDIT_WRONG:
    sprintf(errStr, "Edit config with $EDITOR\nUsage: %s ProgramName or configBasename (or both)", names);
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

void print_parser_help() {
  char names[STR_S];
  printf("ck - the config keeper\n");
  printf("Usage:\n");
  get_possible_action_strings(names, CKA_INIT);
  printf("Initialize: \t%s\n", names);
  get_possible_action_strings(names, CKA_ADD);
  printf("Add config: \t%s\n", names);
  get_possible_action_strings(names, CKA_DEL);
  printf("Delete config: \t%s\n", names);
  get_possible_action_strings(names, CKA_EDIT);
  printf("Edit config: \t%s\n", names);
  get_possible_action_strings(names, CKA_LIST);
  printf("List configs: \t%s\n", names);
  get_possible_action_strings(names, CKA_SEARCH);
  printf("Search: \t%s\n", names);
  get_possible_action_strings(names, CKA_HELP);
  printf("Print this: \t%s\n", names);
}

ActionParseResult parse_action(int argc, char* argv[], UserOpt *opt) {
  /* make empty user opt */
  *opt = make_empty_user_opt();
  if (argc < 2) {
    return APR_HELP;
  }
  opts = argv;
  optNum = argc;

  /* skip the program name */
  next_token();

  /* figure what is the config file */
  get_config(opt);

  /* find the action */
  determine_action(opt);
  if (opt->action == CK_WRONG_ACTION) {
    opt->err = PERR_UNKONW_ACTION;
    return APR_ERR;
  }
  if (opt->action == CKA_HELP) {
    return APR_HELP;
  }

  // parse values
  if (!parse_vals(opt)) {
    return APR_ERR;
  }

  if (opt->err == PERR_NOERR) {
    return APR_OK;
  }
  else {
    return APR_ERR;
  }
}
