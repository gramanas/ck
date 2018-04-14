/* optparser.h - Opt parser for ck -----------------------------*- C -*-
 *
 * This file is part of ck, the config keeper
 *
 * ---------------------------------------------------------------------
 *
 * The code here and in checks.c is responsible for checking
 * if a database file is present and if the user has initialized
 * ck before with correct VC_dir and SCRT_dir paths.
 *
 * It is called before the parsing of the user's arguments
 * in order to make sure that everything is setup properly
 * for the later operations.
 *
 * ------------------------------------------------------------------ */
#ifndef CHECKS_H
#define CHECKS_H

typedef enum CheckResults CheckResult;
enum CheckResults {
  CR_OK,
  CR_NO_CONFIG,
  CR_WRONG_CONFIG,
  CR_NO_DB
};



CheckResult doInitCheck();

#endif // CHECKS_H
