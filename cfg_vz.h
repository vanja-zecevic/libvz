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

#ifndef CFG_VZ_H
    #define CFG_VZ_H

    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>

    typedef struct cfg_vars_struct {
        void * address;
        char name[256];
        int type;
    } cfg_vars_struct;

    #define SUFFIX_(type_macro) SUFFIX_##type_macro
    #define SUFFIX_char [256]
    #define SUFFIX_int
    #define SUFFIX_float
    #define SUFFIX_double

    #define NUM_(type_macro) NUM_PASTE(type_macro)
    #define NUM_PASTE(type_macro) NUM_##type_macro
    #define NUM_char 0
    #define NUM_int 1
    #define NUM_float 2
    #define NUM_double 3

    #define FORMAT_(type_macro) FORMAT_##type_macro
    #define FORMAT_char "%s"
    #define FORMAT_int "%i"
    #define FORMAT_float "%g"
    #define FORMAT_double "%g"

    #define DEFINE_CVARS(var_macro, type_macro) \
      type_macro var_macro SUFFIX_(type_macro);

    #define POPULATE_CVARS(var_macro, type_macro) \
      cfg_vars[iCFG].address = &var_macro; \
      strncpy(cfg_vars[iCFG].name, #var_macro, 256); \
      cfg_vars[iCFG++].type = NUM_(type_macro);

    #define PRINT_CVARS(var_macro, type_macro) \
      printf(#var_macro " = " FORMAT_(type_macro) "\n", var_macro);

    #define COUNT_CVARS(var_macro, type_macro) \
      iCFG++;

    int split_vars(char line[256], char var_str[256], char val_str[256]);
    int read_cvars(char * proj_fname, cfg_vars_struct * cfg_vars, int ncvars);

#endif
