/* actions.h - Action parser for ck ----------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * ---------------------------------------------------------------------
 *
 *
 * ------------------------------------------------------------------ */
#ifndef ACTIONS_H
#define ACTIONS_H

#include "actionparser.h"
#include "confparser.h"

#define X(ACTION)                               \
  extern int run_##ACTION(UserOpt *, Conf *);
CK_ACTIONS
#undef X

#endif /* ACTIONS_H */
