/* actions.h - ck actions ----------------------------------------------*- C -*-
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
#ifndef ACTIONS_H
#define ACTIONS_H

#include "actionparser.h"
#include "confparser.h"

#define X(ACTION)                               \
  extern int run_##ACTION(UserOpt *, Conf *);
CK_ACTIONS
#undef X

#define X(ACTION)                                               \
  extern void print_##ACTION##_result(int ok);
CK_ACTIONS
#undef X

#endif /* ACTIONS_H */
