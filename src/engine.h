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
 *
 *
 * -------------------------------------------------------------------------- */
#include "actions.h"
#include "ckutil.h"

/* if engine_err is set return true
 * along with a copy of the error message.
 * Pass NULL if you don't care about the message */
extern int engine_err_message(char *err);

extern void engine_add_make_link(const AddOpt *opt,
                                 const Conf *conf);


