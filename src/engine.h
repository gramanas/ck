/* engine.h - manages the linking and copying of configs ---------------*- C -*-
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
 * The functions in here are responsible for manipulating
 * the file structure of the system and creating the links.
 *
 * It makes use of some util functions that wrap the actual
 * function calls to linux functions
 *
 * After a call to an engine_*() function one has to check
 * engine_err_message() to check for errors
 *
 * -------------------------------------------------------------------------- */
#include "actions.h"
#include "ckutil.h"

/* if engine_err is set, return true
 * along with a copy of the error message.
 * Pass NULL if you don't care about the message */
extern int engine_err_message(char *err);


extern void engine_add_make_link(const AddOpt *opt,
                                 const Conf *conf);


