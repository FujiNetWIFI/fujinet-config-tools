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

void fcd(void)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xE1;
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=&buf;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=256;
  OS.dcb.daux=0;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      exit(OS.dcb.dstats);
    }
}

int main(int argc, char* argv[])
{  
  OS.lmargn=2;
  
  if (_is_cmdline_dos())
    {
      if (argc<2)
	goto interactive;
      else
	{
	  for (i=1;i<=argc;i++)
	    {
	      strcat(buf,argv[i]);
	      if (i<argc-1)
		strcat(buf," ");
	    }
	}
    }
  else
    {
    interactive:
      // DOS 2.0/MYDOS
      print("HOST PREFIX--ENTER NEW OR \xD2\xC5\xD4\xD5\xD2\xCE TO CLEAR\x9b");
      get_line(buf,240);
    }

  fcd();
  
  return(0);
}
