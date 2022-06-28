/*

    Copyright (C) 2007 by Arlindo da Silva <dasilva@opengrads.org>
    All Rights Reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; using version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, please consult  
              
              http://www.gnu.org/licenses/licenses.html

    or write to the Free Software Foundation, Inc., 59 Temple Place,
    Suite 330, Boston, MA 02111-1307 USA

*/

/*
 * author: Davide Sacchetti (2011)
 * compile with: gcc -fPIC libshell.c -shared -o libshell.gex
 * add in udxt:   udc  0  shell      c_shell    ^libshell.gex "return output of shell cmd"
*/

#include <stdio.h>
#include <stdlib.h>
#include "grads.h"

int c_shell ( int argc, char **argv, struct gacmn *pcm) {
   FILE *fpipe;
   char *command=argv[1];
   char line[256];
   int rc;                                                                                                                                                                   
   if ( !(fpipe = (FILE*)popen(command,"r")) ) {
      perror("Problems with pipe");
      exit(1);
   }
   while ( fgets( line, sizeof line, fpipe)) {
     gaprnt(2, line);
// gaprnt(level,line) scrive su result, se level<2 scrive anche su STDOUT
   }
   pclose(fpipe);

  /* Exit! */
  return(rc);
}

/* torna un CR in result */
int c_CarriageReturn ( int argc, char **argv, struct gacmn *pcm) {
   int rc = 0;
   //char add[1];
   //strcpy(add, "\n");
   gaprnt(2, "\n");
  /* Exit! */
  return(rc);
}
