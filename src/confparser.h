/* confparser.h - Configuration file parser for ck ---------------------*- C -*-
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
 * The code here and in confparser.c is responsible parsing
 * the configuration and get the values set there
 *
 * -------------------------------------------------------------------------- */
#ifndef CONFPARSER_H
#define CONFPARSER_H

#include "actionparser.h"

#define CONFIG_VARIABLES_TABLE                                          \
  X(VC_dir,   " version_control_dir = %s ", "Version Control directory") \
  X(SCRT_dir, " secret_dir = %s "         , "Secret directory")


typedef enum ConfingVariables ConfVar;
#define X(var, str, name) CV_##var,
enum ConfingVariables {
  CV_NO_VAL_OR_COMMENT,
  CONFIG_VARIABLES_TABLE
};
#undef X

typedef enum ConfigParserResults ConfigParserResult;
#define X(var, str, name)                       \
  CPR_WRONG_##var,
enum ConfigParserResults {
  CPR_OK,
  CPR_NO_CONFIG_FILE,
  CPR_WRONG_CONFIG,
  CONFIG_VARIABLES_TABLE
};
#undef X

typedef struct ConfigValues Conf;
#define X(var, str, name) char* var;
struct ConfigValues {
  ConfigParserResult result;
  CONFIG_VARIABLES_TABLE
};
#undef X

extern int config_file_parse(Conf *conf, UserOpt *opt);

extern int init_create_config_file(UserOpt *opt);
#endif // CONFPARSER_H
