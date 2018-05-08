#include "ckutil.h"

typedef enum edit_opt_return_code edit_rc;
enum edit_opt_return_code {
  ERC_OK,
  ERC_SUGGESTIONS,
  ERC_ERR
};

extern edit_rc edit_get_config_or_suggestions(const int argc, char **argv, char *ret);
