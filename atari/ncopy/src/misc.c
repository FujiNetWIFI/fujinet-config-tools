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
#include "context.h"
#include "misc.h"
#include "conio.h"
#include "cio.h"

extern char sourcePathSeperator;
extern char destPathSeperator;

void print_error(unsigned char err)
{
  unsigned char errnum[4];
  print("ERROR- ");
  itoa(err,errnum,10);
  print(errnum);
  print("\x9b");
}

void detect_seperator()
{
  if (strchr(sourceDeviceSpec,'/') != NULL)
    sourcePathSeperator = '/';
  else if (strchr(sourceDeviceSpec,'\\') != NULL)
    sourcePathSeperator = '\\';
  else
    sourcePathSeperator = ':';

  if (strchr(destDeviceSpec,'/') != NULL)
    destPathSeperator = '/';
  else if (strchr(destDeviceSpec,'\\') != NULL)
    destPathSeperator = '\\';
  else
    destPathSeperator = ':';
}

bool detect_wildcard(char* b)
{
  if (strchr(b,'*')!=NULL)
    return true;
  if (strchr(b,'?')!=NULL)
    return true;

  return false;  
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

  if (aux1 != 0)
    OS.iocb[channel].aux1=aux1;
  
  return ciov(channel);
}
