/**
 * Network Testing tools
 *
 * ndir - return N: directory.
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
unsigned char daux1=0;
unsigned char daux2=0;
unsigned char i=0;

/**
 * Wait for keypress
 */
void pause(void)
{
  if (!_is_cmdline_dos())
    {
      print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf,sizeof(buf));
    }
}

void nopen(unsigned char unit)
{
  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd='O';
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=&buf;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=256;
  OS.dcb.daux1=6;
  OS.dcb.daux2=128; // Long dir.
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      pause();
      exit(OS.dcb.dstats);
    }
}

void nclose(unsigned char unit)
{
  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd='C';
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=NULL;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=0;
  OS.dcb.daux1=0;
  OS.dcb.daux2=0;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      pause();
      exit(OS.dcb.dstats);
    }
}

void nread(unsigned char unit, unsigned short len)
{
  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd='R';
  OS.dcb.dstats=0x40;
  OS.dcb.dbuf=&buf;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=len;
  OS.dcb.daux1=len;
  OS.dcb.daux2=0;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      pause();
      exit(OS.dcb.dstats);
    }
}

void nstatus(unsigned char unit)
{
  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd='S';
  OS.dcb.dstats=0x40;
  OS.dcb.dbuf=&OS.dvstat;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=4;
  OS.dcb.daux1=0;
  OS.dcb.daux2=0;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      pause();
      exit(OS.dcb.dstats);
    }
}

int main(int argc, char* argv[])
{
  unsigned char u=1;
  unsigned short ab=0;
  
  OS.lmargn=2;
  
  if (_is_cmdline_dos())
    {
      if (argc<2)
	goto interactive;
      else
	{
	  for (i=1;i<argc;i++)
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
      print("\x9b");
      print("ENTER PATH, WILDCARD OR \xD2\xC5\xD4\xD5\xD2\xCE\x9b");
      get_line(buf,240);
    }

  // if no device, set a device path.
  if ((buf[1]!=':') && (buf[2]!=':'))
    {
      memmove(&buf[2],&buf[0],sizeof(buf)-3);
      buf[0]='N';
      buf[1]=':';
    }
  else if (buf[2]==':')
    u=buf[1]-0x30;

  nopen(u);

  do {
    nstatus(u);
    
    ab=OS.dvstat[1]*256+OS.dvstat[0];

    if (ab>255)
      ab=255;
    else if (ab==0)
      break;

    memset(&buf,0,sizeof(buf));

    nread(u,ab);

    print(buf);
    
  } while(ab!=0);

  nclose(u);

  pause();
  
  return(0);
}
