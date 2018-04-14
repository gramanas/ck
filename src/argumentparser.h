/* argumentparser.h - Argument parser for ck------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * ---------------------------------------------------------------------
 *
 * The code here and in argumentparser.c is responsible for parsing 
 * the user's input from the command line 
 *
 * Keeps track of what error occured where and provides
 * printParserHelp() and printParserError() functions
 * to notify the user
 *
 * ------------------------------------------------------------------ */
#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H
#define INIT_ALIAS                              \
  

#define ARGUMENT_TABLE                          \
  X(init, )

typedef enum ParseResults ParseResult;

extern ParseResult parse(int argc, char *argv[]);

#endif // ARGUMENTPARSER_H
