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

    typedef struct {
        void * address;
        char name[256];
        int type;
        int found;
    } cfg_vars_struct;

    #define CFG_SUFFIX_(type_macro) CFG_SUFFIX_##type_macro
    #define CFG_SUFFIX_char [256]
    #define CFG_SUFFIX_int
    #define CFG_SUFFIX_float
    #define CFG_SUFFIX_double

    #define CFG_NUM_(type_macro) CFG_NUM_PASTE(type_macro)
    #define CFG_NUM_PASTE(type_macro) CFG_NUM_##type_macro
    #define CFG_NUM_char 0
    #define CFG_NUM_int 1
    #define CFG_NUM_float 2
    #define CFG_NUM_double 3

    #define CFG_FORMAT_(type_macro) CFG_FORMAT_##type_macro
    #define CFG_FORMAT_char "%s"
    #define CFG_FORMAT_int "%i"
    #define CFG_FORMAT_float "%g"
    #define CFG_FORMAT_double "%g"

    #define CFG_DEFAULT_(type_macro) CFG_DEFAULT_##type_macro
    #define CFG_DEFAULT_char "ds"
    #define CFG_DEFAULT_int 0 
    #define CFG_DEFAULT_float 0. 
    #define CFG_DEFAULT_double 0.f

    #define DEFINE_CVARS(var_macro, type_macro) \
      type_macro var_macro CFG_SUFFIX_(type_macro) = CFG_DEFAULT_(type_macro);

    #define POPULATE_CVARS(var_macro, type_macro) \
      cfg_vars[iCFG].address = &var_macro; \
      strncpy(cfg_vars[iCFG].name, #var_macro, 256); \
      cfg_vars[iCFG].type = CFG_NUM_(type_macro); \
      cfg_vars[iCFG++].found = 0;

    #define PRINT_CVARS(var_macro, type_macro) \
      printf(#var_macro " = " CFG_FORMAT_(type_macro) "\n", var_macro);

    #define COUNT_CVARS(var_macro, type_macro) \
      iCFG++;

    int split_vars(char line[256], char var_str[256], char val_str[256]);
    int read_cvars(char * proj_fname, cfg_vars_struct * cfg_vars, int ncvars);
    void report_unread(cfg_vars_struct * cfg_vars, int ncvars);

#endif
