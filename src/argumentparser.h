/* argumentparser.h - Argument parser for ck------------------------*- C -*- */
#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H
#define INIT_ALIAS                              \
  

#define ARGUMENT_TABLE                          \
  X(init, )

typedef enum ParseResults ParseResult;

extern ParseResult parse(int argc, char *argv[]);

#endif // ARGUMENTPARSER_H
