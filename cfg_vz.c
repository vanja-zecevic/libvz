/* Copyright (C) 2012 Vanja Zecevic
   Contact vanja.zecevic@sydney.uni.edu.au

   This file is part of lib_vz

   lib_vz is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   lib_vz is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */

/* =============================================================================
   lib_vz/cfg_vz.c
                                  ----------------------------------------------

   This program is used to read a configuration file and store the results in
   variables with corresponding names that are then available in the main
   program. 
   Any errors in the configuration file syntax are silent, however errors can be
   checked by printing the list of variables expected to be read in and looking
   for errors.

   -----------------------------------------------------------------------------
   Example usage, manual entry:
                                        ----------------------------------------

    #include "libvz/cfg_vz.h"
    int iCFG;
    cfg_vars_struct * cfg_vars;
    #define X_MACRO(AA, BB) DEFINE_CVARS(AA, BB)
    #include CVARS_FILE   

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

   -----------------------------------------------------------------------------
   Example usage, include files:
                                        ----------------------------------------

    #include "libvz/cfg_vz.h"
    #include "libvz/cfg_def.inc"
    #include "libvz/cfg_parse.inc"

   -----------------------------------------------------------------------------
   CVARS_FILE:
                                        ----------------------------------------

    X_MACRO(some_float_variable,    PREC)
    X_MACRO(another_float_variable, PREC)
    X_MACRO(some_int_variable,      int)
    #undef X_MACRO

*/ 
#include "libvz/cfg_vz.h"
#include "libvz/exit_vz.h"
/*============================================================================*/
int read_cvars(char * proj_fname, cfg_vars_struct * cfg_vars, int ncvars) {

FILE * proj_file;

char cur_line[256];
char var_str[256];
char val_str[256];

int ivar;
int iln;

/* Attempt to open the input file */
proj_file = fopen(proj_fname, "r");
printf("Opening config file, %s\n", proj_fname);
if (proj_file == NULL) exit_msg("Error opening config file", 1);

/* Parse the config file.  */
for (iln=0; iln<1024 && fgets(cur_line, 256, proj_file)!=NULL; iln++) {
    split_vars(cur_line, var_str, val_str);
    for (ivar=0; ivar<(ncvars-1) && strcmp(cfg_vars[ivar].name, var_str)!=0
         ; ivar++);

    switch (cfg_vars[ivar].type) {
    case 0:
        strncpy(cfg_vars[ivar].address, val_str, 256);
        *((char*)cfg_vars[ivar].address+ 255) = '\0';
        break;
    case 1:
        *((int*)cfg_vars[ivar].address) = atoi(val_str);
        break;
    case 2:
        *((float*)cfg_vars[ivar].address) = atof(val_str);
        break;
    case 3:
        *((double*)cfg_vars[ivar].address) = atof(val_str);
        break;
    default:
        exit_msg("No type found?", 1);    
}
}
return 0;
}
/*===========================================================================*/
int split_vars(char line[256], char var_str[256], char val_str[256])
/* Splits config file lines at according to the following pattern;
   space var_str space val_str
   Anything after val_str is discarded.
   Returns 0 if valid pattern found, 1 otherwise.
   Lines beginning with # are discarded.  */
{
int iln = 0;
int ivar = 0;
int ival = 0;

/* Skip comments.  */
if (*line == '#') {
    *var_str = '\0';
    *val_str = '\0';
    return 1;
}

/* Eats any preceeding space.  */
for ( ; iln<255 && (line[iln]==' ' || line[iln]=='\t'); iln++);

/* Add characters to the 'var' until you encounter a space or null.  */
for ( ; iln<255 && line[iln]!=' ' && line[iln]!='\0' && line[iln]!='\n'
      ; iln++, ivar++)
  var_str[ivar] = line[iln];
var_str[ivar] = '\0';

/* Eats any more space.  */
for ( ; iln<255 && (line[iln]==' ' || line[iln]=='\t'); iln++);

/* Add characters to the 'val' until you encounter a space or null.  */
for ( ; iln<255 && line[iln]!=' ' && line[iln]!='\0' && line[iln]!='\n'
  ; iln++, ival++)
  val_str[ival] = line[iln];
val_str[ival] = '\0';

//printf("%s %i\n%s %i\n",var_str, ivar, val_str, ival);
if (ivar!=0 && ival!=0) return 0;
else return 1;

}
/*============================================================================*/

