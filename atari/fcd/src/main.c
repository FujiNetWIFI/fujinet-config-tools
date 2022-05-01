/**
 * Network Testing tools
 *
 * fcd - set FUJI HOST SLOT prefix.
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Released under GPL 3.0
 * See COPYING for details.
 */

#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include <peekpoke.h>
#include "sio.h"
#include "conio.h"
#include "err.h"

unsigned char buf[256];
unsigned char i=0;

unsigned char fcd(unsigned char hs, const char* prefix)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xE1;
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=prefix;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=256;
  OS.dcb.daux1=hs;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
    }
  return OS.dcb.dstats;
}

int main(int argc, char* argv[])
{
  unsigned char i;
  char *tokens[2];
  unsigned char hs;
  unsigned char err;
  
  OS.lmargn=2;
  
  if (_is_cmdline_dos())
    {
      for (i=1;i<argc;i++)
	{
	  strcat(buf,argv[i]);
	  if (i<argc-1)
	    strcat(buf," ");
	}
    }
  else
    {
      // DOS 2.0/MYDOS
      print("HOST PREFIX--HOST SLOT, PATH?\x9b");
      get_line(buf,240);
    }

  tokens[0]=strtok(buf,",");
  tokens[1]=strtok(NULL,",");

  hs=atoi(tokens[0]);
  
  if (tokens[0]==NULL)
    {
      print("HOST SLOT REQUIRED\x9b");
      err=132;
    }
  else if (hs<1 || hs>8)
    {
      print("INVALID HOST SLOT NUMBER\x9b");
      err=160;
    }
  else
    {
      err=fcd(hs-1,tokens[1]);
    }

  if (err!=1 && !_is_cmdline_dos())
    {
      print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf,sizeof(buf));
    }
  
  return err==1 ? 0 : err;
}
