#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include "checks.h"



/* const char* const matchVCDirName = " version_control_dir = %s"; */
/* const char* const matchSCRTDirName = " secret_dir = %s"; */
/* const char* const matchDBDirName = " database_dir = %s"; */

/* ConfigParserResult parseConfigFile(FILE *f, Conf *c) { */
/*   char *vc_dir, *scrt_dir, *db_dir; */
/*   DIR *dir; */
  
/*   char * buffer = 0; */
/*   long length; */
/*   if (f) { */
/*     fseek(f, 0, SEEK_END); */
/*     length = ftell (f); */
/*     fseek(f, 0, SEEK_SET); */
/*     buffer = malloc (length); */
/*     if (buffer) { */
/*       fread (buffer, 1, length, f); */
/*       for (int i = 0; i < length; i++) { */
/*         if (buffer[i] == '\n') { */
/*           buffer[i] = ' '; */
/*         } */
/*       } */
/*     } */
/*     fclose (f); */
/*   } */
  
  /* if (buffer) { */
  /*   // VC dir */
  /*   vc_dir = malloc(sizeof(char)*100); */
  /*   if (sscanf(buffer, matchVCDirName, vc_dir) != 1) { */
  /*     return CPR_NO_VC_DIR; */
  /*   } */
  /*   dir = opendir(vc_dir); */
  /*   if (!dir) { */
  /*     return CPR_WRONG_VC_DIR; */
  /*   } */
  /*   closedir(dir); */
  /*   c->VC_dir = vc_dir; */

/*     /\* printf("%s", buffer); *\/ */
    
/*     // SCRT_dir */
/*     scrt_dir = malloc(sizeof(char)*100); */
/*     if(sscanf(buffer, matchSCRTDirName, scrt_dir) != 1) { */
/*       return CPR_NO_SCRT_DIR; */
/*     } */
/*     dir = opendir(scrt_dir); */
/*     if (!dir) { */
/*       return CPR_WRONG_SCRT_DIR; */
/*     } */
/*     c->SCRT_dir = scrt_dir; */

/*     // DB_dir */
/*     db_dir = malloc(sizeof(char)*100); */
/*     if (sscanf(buffer, matchDBDirName, db_dir) != 1) { */
/*       return CPR_NO_DB_DIR; */
/*     } */
/*     dir = opendir(db_dir); */
/*     if (!dir) { */
/*       return CPR_WRONG_DB_DIR; */
/*     } */
/*     c->DB_dir = db_dir;     */
/*   } */
/*   return CPR_OK; */
/* } */
    

/* char *getConfigPath() { */
/*   char *home = getenv("HOME"); */
/*   char *path; */
/*   if (home == NULL) { */
/*     printf("HOME variable is not set. Can't find config."); */
    
/*   } */
/*   if ((path = malloc(strlen(home)+strlen(configFilename)+1 /\* for the slash *\/))) { */
/*     strcat(path, home); */
/*     strcat(path, "/"); */
/*     strcat(path, configFilename); */
/*   } */
/*   return path; */
/* } */

// -1 config file problem
// 0 config content problem
// 1 all good
/* int doConfigCheck(Conf *conf) { */
/*   char* ckConfigPath = getConfigPath(); */
/*   if (ckConfigPath == NULL) { */
/*     return -1; */
/*   } */
/*   FILE *confPtr; */
/*   if ((confPtr = fopen(ckConfigPath, "r")) == NULL){ */
/*     return -1; */
/*   } */
/*   return 2; */
/* } */

/* /\* CheckResult doInitCheck(Conf *conf) { */ 
  /* switch (doConfigCheck(conf)) { */
  /* case CPR_NO_VC_DIR: */
  /*   printf("There is no VersionControl directory defined in the configuration file.\n"); */
  /*   return CR_NO_CONFIG; */
  /* case CPR_WRONG_VC_DIR: */
  /*   printf("The VersionControl directory defined in the configuration file is wrong.\n"); */
  /*   return CR_WRONG_CONFIG; */
  /* case CPR_NO_SCRT_DIR: */
  /*   printf("There is no Secret directory defined in the configuration file.\n"); */
  /*   return CR_NO_CONFIG; */
  /* case CPR_WRONG_SCRT_DIR: */
  /*   printf("The Secret directory defined in the configuration file is wrong.\n"); */
  /*   return CR_WRONG_CONFIG; */
  /* case CPR_NO_DB_DIR: */
  /*   printf("There is no Database directory defined in the configuration file.\n"); */
  /*   return CR_NO_CONFIG; */
  /* case CPR_WRONG_DB_DIR: */
  /*   printf("The Database directory defined in the configuration file is wrong.\n"); */
  /*   return CR_WRONG_CONFIG; */
  /* case CPR_OK: */
  /*   break; */
  /* } */
//  return 0;
  //doDbCheck();
//}
