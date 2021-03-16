/**
 * Network Testing tools
 *
 * ncopy - copy files 
 *  N:<->D: D:<->N: or N:<->N:
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Released under GPL 3.0
 * See COPYING for details.
 */

#include <atari.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "misc.h"
#include "conio.h"

extern char errnum[4];
extern unsigned char yvar;

void print_error(void)
{
  print("ERROR- ");
  itoa(yvar,errnum,10);
  print(errnum);
  print("\x9b");
}

bool detect_wildcard(char* buf)
{
  char *pWildcardStar;
  char *pWildcardChar;
  
  pWildcardStar=strchr(buf, '*');
  pWildcardChar=strchr(buf, '?');
  return ((pWildcardStar!=NULL) || (pWildcardChar!=NULL));
}
