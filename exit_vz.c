/* Author Vanja Zecevic
   Contact vanja.zecevic@sydney.uni.edu.au

   This file is part of lib_vz however it is not copyrighted. */

/* =============================================================================
   lib_vz/exit_vz.c
                                  ----------------------------------------------
   This little function is often used and I didn't know where to put it other
   than in its own file :)
*/

#include "libvz/exit_vz.h"
void exit_msg(char * message, int exit_value)
{
printf("%s", message);
printf(", exiting...\n");
exit(exit_value);
}

