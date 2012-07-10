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

#ifndef LLIFFE_VZ_H
	#define LLIFFE_VZ_H

    #include <malloc.h>
    #include <stdio.h>

    #include "libvz/exit_vz.h"

    typedef struct {
        float **** qua;
        float *** ter;
        float ** sec;
        float * pri;
    } lliffe_4d_float;

    typedef struct {
        float *** ter;
        float ** sec;
        float * pri;
    } lliffe_3d_float;

    typedef struct {
        float ** sec;
        float * pri;
    } lliffe_2d_float;

    typedef struct {
        double **** qua;
        double *** ter;
        double ** sec;
        double * pri;
    } lliffe_4d_double;

    typedef struct {
        double *** ter;
        double ** sec;
        double * pri;
    } lliffe_3d_double;

    typedef struct {
        double ** sec;
        double * pri;
    } lliffe_2d_double;

    int malloc_2d_float (lliffe_2d_float * ptr_return,  int ny, int nx);
    int malloc_2d_double(lliffe_2d_double * ptr_return, int ny, int nx);
    int malloc_3d_float (lliffe_3d_float * ptr_return,  int nz, int ny, int nx);
    int malloc_3d_double(lliffe_3d_double * ptr_return, int nz, int ny, int nx);
    int malloc_4d_float (lliffe_4d_float * ptr_return,  int nw, int nz, int ny,
      int nx);
    int malloc_4d_double(lliffe_4d_double * ptr_return, int nw, int nz, int ny,
      int nx);

    void free_2d_float(lliffe_2d_float ptr);
    void free_3d_float(lliffe_3d_float ptr);
    void free_4d_float(lliffe_4d_float ptr);

    void free_2d_double(lliffe_2d_double ptr);
    void free_3d_double(lliffe_3d_double ptr);
    void free_4d_double(lliffe_4d_double ptr);

    #define MALLOC_XD_PASTE(A, B) A##B

    #define lliffe_2d_(A) MALLOC_XD_PASTE(lliffe_2d_,A)
    #define lliffe_3d_(A) MALLOC_XD_PASTE(lliffe_3d_,A)
    #define lliffe_4d_(A) MALLOC_XD_PASTE(lliffe_4d_,A)

    #define malloc_2d_(A) MALLOC_XD_PASTE(malloc_2d_,A)
    #define malloc_3d_(A) MALLOC_XD_PASTE(malloc_3d_,A)
    #define malloc_4d_(A) MALLOC_XD_PASTE(malloc_4d_,A)

    #define free_2d_(A) MALLOC_XD_PASTE(free_2d_,A)
    #define free_3d_(A) MALLOC_XD_PASTE(free_3d_,A)
    #define free_4d_(A) MALLOC_XD_PASTE(free_4d_,A)


#endif
