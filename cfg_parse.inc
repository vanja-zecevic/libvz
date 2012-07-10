iCFG = 0;
#define X_MACRO(AA, BB) COUNT_CVARS(AA, BB)
#include CVARS_FILE
cfg_vars = (cfg_vars_struct*)malloc(iCFG*sizeof(cfg_vars_struct));

iCFG = 0;
#define X_MACRO(AA, BB) POPULATE_CVARS(AA, BB)
#include CVARS_FILE

read_cvars(projfilename, cfg_vars, iCFG);
#define X_MACRO(AA, BB) PRINT_CVARS(AA, BB)
#include CVARS_FILE
