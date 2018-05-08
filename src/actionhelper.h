
/* actionhelper.h - helper functions for ck actions --------------------*- C -*-
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
 * -------------------------------------------------------------------------- */
#include "actions.h"
#include "ckutil.h"
#include "cklist.h"

typedef enum edit_opt_return_code edit_rc;
enum edit_opt_return_code {
  ERC_OK,
  ERC_SUGGESTIONS,
  ERC_ERR
};

/* if add_err is set, return true
 * along with a copy of the error message.
 * Pass NULL if you don't care about the message */
extern int add_err_message(char *err);


extern void add_make_link(const AddOpt *opt,
                                 const Conf *conf);

extern edit_rc edit_get_config_or_suggestions(cklist* args, char *ret);
