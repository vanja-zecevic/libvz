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

#if defined (LLIFFE_VZ_H)
/*============================================================================*/
int malloc_2d_(PREC) (lliffe_2d_(PREC) * ptr, int ny, int nx)
{
int i;
int j;

/* Memory block that holds allof the data.  */
ptr->pri = (PREC*)malloc(nx*ny*sizeof(PREC)); 
if (ptr->pri == NULL) exit_msg("malloc2d error", 1);

/* Memory block that holds pointers 
 * to the start of each x vector for use with array notation.  */
ptr->sec = (PREC**)malloc(ny*sizeof(PREC*));
if (ptr->sec == NULL) exit_msg("malloc2d error", 1);

/* Points elements of ptr_array so that it is now defined
 * dynamically in an equivalent way to ptr_sec[ny][nx].  */
for (j=0; j<ny; j++) (ptr->sec)[j] = ptr->pri + nx*j; 
		
/* Initialise to 0's.  */
for (i=0; i<nx; i++) for (j=0; j<ny; j++) (ptr->sec)[j][i] = (PREC)0; 

return 0;
}
/*============================================================================*/
int malloc_3d_(PREC) (lliffe_3d_(PREC) * ptr, int nz, int ny, int nx)
{
int i;
int j;
int k;

/* Memory block that holds all of the data.  */
ptr->pri = (PREC*)malloc(nx*ny*nz*sizeof(PREC)); 
if (ptr->pri == NULL) exit_msg("malloc3d error", 1);

/* Memory block that holds pointers
 * to the start of each x vector for use with array notation.  */
ptr->sec = (PREC**)malloc(ny*nz*sizeof(PREC *));
if (ptr->sec == NULL) exit_msg("malloc3d error", 1);

/* Memory block that holds pointers
 * to the start of each x-y sub-array for use with array notation.  */
ptr->ter = (PREC***)malloc(nz*sizeof(PREC **));
if (ptr->ter == NULL) exit_msg("malloc3d error", 1);

/* Points elements of ptr_array so that it is now defined
 * dynamically in an equivalent way to ptr_sec[ny][nx].  */
for (j=0; j<ny*nz; j++) (ptr->sec)[j] = ptr->pri + nx*j; 

/* Points elements of ptr_array so that it is now defined
 * dynamically in an equivalent way to ptr_ter[nz][ny][nx].  */
for (k=0; k<nz; k++) (ptr->ter)[k] = ptr->sec + ny*k; 

/* Initialise to 0's.  */
for (i=0; i<nx; i++) for (j=0; j<ny; j++) for (k=0; k<nz; k++) 
  (ptr->ter)[k][j][i] = (PREC)0;

return 0;
}
/*============================================================================*/
int malloc_4d_(PREC) (lliffe_4d_(PREC) * ptr, int nw, int nz, int ny, int nx)
{
int i;
int j;
int k;
int l;

/* Memory block that holds all of the data.  */
ptr->pri = (PREC*)malloc(nx*ny*nz*nw*sizeof(PREC));
if (ptr->pri == NULL) exit_msg("malloc4d error", 1);

/* Memory block that holds pointers 
 * to the start of each x vector for use with array notation.  */
ptr->sec = (PREC**)malloc(ny*nz*nw*sizeof(PREC *)); 
if (ptr->sec == NULL) exit_msg("malloc4d error", 1);

/* Memory block that holds pointers 
 * to the start of each x-y sub-array for use with array notation.  */
ptr->ter = (PREC***)malloc(nz*nw*sizeof(PREC **)); 
if (ptr->ter == NULL) exit_msg("malloc4d error", 1);

/* Memory block that holds pointers 
 * to the start of each x-y sub-array for use with array notation.  */
ptr->qua = (PREC****)malloc(nw*sizeof(PREC ***)); 
if (ptr->qua == NULL) exit_msg("malloc4d error", 1);

/* Points elements of ptr_array so that it is now defined
 * dynamically in an equivalent way to ptr_sec[ny][nx].  */
for (j=0; j<ny*nz*nw; j++) (ptr->sec)[j] = ptr->pri + nx*j;

/* Points elements of ptr_array so that it is now defined
 * dynamically in an equivalent way to ptr_ter[nz][ny][nx].  */
for (k=0; k<nz*nw; k++) (ptr->ter)[k] = ptr->sec + ny*k; 

/* Points elements of ptr_array so that it is now defined
 * dynamically in an equivalent way to ptr_qua[nw][nz][ny][nx].  */
for (l=0; l<nw; l++) (ptr->qua)[l] = ptr->ter + nz*l; 

/* Initialise to 0's.  */
for (i=0; i<nx; i++) for (j=0; j<ny; j++) for (k=0; k<nz; k++) 
  for (l=0; l<nw; l++) (ptr->qua)[l][k][j][i] = (PREC)0; 

return 0;
}
/*============================================================================*/
void free_2d_(PREC) (lliffe_2d_(PREC) ptr)
{
free(ptr.pri);
free(ptr.sec);
}
/*============================================================================*/
void free_3d_(PREC) (lliffe_3d_(PREC) ptr)
{
free(ptr.pri);
free(ptr.sec);
free(ptr.ter);
}
/*============================================================================*/
void free_4d_(PREC) (lliffe_4d_(PREC) ptr)
{
free(ptr.pri);
free(ptr.sec);
free(ptr.ter);
free(ptr.qua);
}
/*============================================================================*/
#endif
