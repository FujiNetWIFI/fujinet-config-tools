/**
 * Network Testing tools
 *
 * ntrans - Set network translation parameter
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

unsigned char daux2=0;
unsigned char i=0;
unsigned char buf[128];
unsigned char transtmp[4];

void ntrans(unsigned char unit, unsigned char mode)
{
  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd='T'; // Set translation
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=NULL;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=0;
  OS.dcb.daux2=mode;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      if (!_is_cmdline_dos())
        {
          print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
          get_line(buf,sizeof(buf));
        }
      exit(OS.dcb.dstats);
    }
}

void opts(void)
{
  print("ntrans <Nx:> <mode>\x9b\x9b");
  print("  <Nx:> device\x9b");
  print("  <mode> 0=None, 1=CR, 2=LF, 3=CR/LF\x9b");
}

int main(int argc, char* argv[])
{
  unsigned char tmp[2]={0,0};
  unsigned char u=1;
  unsigned char m=0;
  
  OS.lmargn=2;
  
  if (_is_cmdline_dos())
    {
      if (argc<3)
	{
	  opts();
	  exit(1);
	}
      else
	{
	  strcpy(buf,argv[1]);
	  strcpy(transtmp,argv[2]);
	}
    }
  else
    {
      // DOS 2.0/MYDOS
      print("\x9b");
      print("SET TRANSLATION--WHICH N:?\x9b");
      get_line(buf,128);
      print("\x9b");
      print("MODE? 0=NONE, 1=CR, 2=LF, 3=CR/LF\x9b");
      get_line(transtmp,3);
    }

  // if no device, set a device path.
  if ((buf[0]>0x30) && (buf[0]<0x39))
    u=buf[0]-0x30;
  else if (buf[2]==':')
    u=buf[1]-0x30;

  m=atoi(transtmp);

  ntrans(u,m);

  return(0);
}
