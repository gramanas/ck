/* actionparser.h - Action parser for ck -------------------------------*- C -*-
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
 * The code here and in actionparser.c is responsible for parsing 
 * the user's input from the command line and return a struct
 * of the user's options ready to be handled by the rest of the 
 * procedures.
 *
 * Keeps track of what error occured where and provides
 * printParserHelp() and printParserError() functions
 * to notify the user
 *
 * -------------------------------------------------------------------------- */
#ifndef ACTIONPARSER_H
#define ACTIONPARSER_H

#define CK_ACTIONS                                                   \
  X(INIT)                                                            \
  X(ADD)                                                             \
  X(DEL)                                                             \
  X(EDIT)                                                            \
  X(LIST)                                                            \
  X(SEARCH)                                                          \
  X(HELP)

typedef enum ParseErrors ParseError;
enum ParseErrors {
  PERR_NOERR = 0,
#define X(ACTION) \
  PERR_##ACTION##_WRONG,
  CK_ACTIONS
#undef X
  PERR_UNKONW_ACTION,
};

typedef enum CkActions CkAction;
enum CkActions {
#define X(ACTION)                               \
  CKA_##ACTION,
  CK_ACTIONS
#undef X
};

typedef enum OptParserResults ParseResult;
enum OptParserResults {
  OPR_OK,
  OPR_ERR,
  OPR_HELP
};

typedef struct UserOptions UserOpt;
struct UserOptions {
  ParseError err;
  CkAction action;
  char *confDir;
  int argc;
  char *argv[10]; // action's options
};


extern ParseResult parseAction(int argc, char* argv[], UserOpt *opt);
extern void printParserError();
extern void printParserHelp();
extern void free_user_opt(UserOpt *opt);

#endif // ACTIONPARSER_H
