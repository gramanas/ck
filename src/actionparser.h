/* actionparser.h - Action parser for ck------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * ---------------------------------------------------------------------
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
 * ------------------------------------------------------------------ */
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
  CKA_INIT,
  CKA_ADD,      // program, path, primary, secret
  CKA_DEL,      // program regexp, if only programm, delete everything related
  CKA_EDIT,     // program regexp, if only program, edit primary
  CKA_LIST,     // list_type{tree,paths,programs}
  CKA_SEARCH,   // search_mode, regexp
  CKA_HELP      // help
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
  int argc;
  char *argv[10]; // action's options
};


extern ParseResult parseAction(int argc, char* argv[], UserOpt *opt);
extern void printParserError();
extern void printParserHelp();

#endif // ACTIONPARSER_H
