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
   lib_vz/lliffe_vz.c
                                  ----------------------------------------------

   This program is used to declare, allocate and free dynamic multi-dimensional
   arrays using lliffe vectors. Accessing multi-dimensional data in this way may
   have different performance characteristics than accessing data via pointers
   and strided offset.

   Strided access in 3D:
   *(var + nX*nY*iZ + nX*iY + iX)
   This results in one offset indirection, three multiplications and two
   additions (in addition to the addition in the pointer indirection).

   Lliffe vector access in 3D:
   var[iZ][iY][iX] = *(*(*(var + iX) + iY) + iZ)
   This results in 3 offset indirections

   -----------------------------------------------------------------------------
   Example usage:
                                        ----------------------------------------

    #include "libvz/lliffe_vz.h"
    lliffe_3d_(double) x_struct;
    double *** x = NULL;
    double * x_ptr = NULL;
    double y;

    malloc_3d_(double) (&x_struct, nZ, nY, nX);
    x = x_struct.ter;
    x_ptr = x_struct.pri;

    y = x[iZ][iY][iX];
    y = *(x_ptr + nX*nY*iZ + nX*iY + iX);

*/
#include "libvz/lliffe_vz.h"

#ifndef PREC
	#define PREC float
	#include "libvz/lliffe_vz.inc"
	#undef PREC
	
        #define PREC double
	#include "libvz/lliffe_vz.inc"
	#undef PREC

#endif


