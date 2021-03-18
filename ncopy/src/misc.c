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
#include "cio.h"

void print_error(unsigned char err)
{
  unsigned char errnum[4];
  print("ERROR- ");
  itoa(err,errnum,10);
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

bool valid_network_device(char d)
{
  return (d=='N');
}

bool valid_cio_device(char d)
{
  return (d!='N' && (d>0x40 && d<0x5B));
}

unsigned char _cio(unsigned char channel, unsigned char command, unsigned char aux1, char* buf, unsigned short len)
{
  OS.iocb[channel].buffer=buf;
  OS.iocb[channel].buflen=len;
  OS.iocb[channel].command=command;
  OS.iocb[channel].aux1=aux1;
  return ciov(channel);  
}
