/**
 * FujiNet Tools for CLI
 *
 * rbaudlok - Enable TCP port XXXX for listening
 *
 * usage:
 *  rbaudlok <port#>
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Released under GPL, see COPYING
 * for details
 */

#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include "sio.h"
#include "conio.h"
#include "err.h"

unsigned char buf[40];
unsigned char newBaud=0;
unsigned char baudLock=0xFF;

/**
 * SIO command to set baud lock.
 */
unsigned char baud_lock(unsigned char l)
{
  OS.dcb.ddevic=0x50;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd='N';
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=NULL;
  OS.dcb.dtimlo=0x01;
  OS.dcb.dbyt=0;
  OS.dcb.daux1=l;

  siov();

  if (OS.dcb.dstats!=1)
    err_sio();

  return OS.dcb.dstats;
}

/**
 * SIO command to configure baud rate
 */
unsigned char configure(unsigned char b)
{
  OS.dcb.ddevic=0x50;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd='B';
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=NULL;
  OS.dcb.dtimlo=0x01;
  OS.dcb.dbyt=0;
  OS.dcb.daux1=b;

  siov();

  if (OS.dcb.dstats!=1)
    err_sio();

  return OS.dcb.dstats;
}


/**
 * show options
 */
void opts(char* argv[])
{
  print(argv[0]);
  print(" <baud|UNLOCK>\x9b\x9b");
  print("Values: either Unlock, or one of:\x9b");
  print("300, 1200, 2400, 4800, 9600, 19200\x9b");
}

/**
 * Parse option
 */
void parseopt(char* o)
{
  // Set whether to enable/disable baud lock.
  if (o[0]=='U')
    {
      baudLock=0;
    }
  else
    {
      baudLock=1;
    }

  // param was baud rate
  switch (o[0])
    {
    case 3: // 300
      newBaud=0x08;
      break;
    case 6: // 600
      newBaud=0x09;
      break;
    case 1: // 1200 or 19200
      newBaud=(o[1]==2 ? 0x0a : 0x0f);
      break;
    case 2: // 2400
      newBaud=0x0c;
      break;
    case 4: // 4800
      newBaud=0x0d;
      break;
    case 9: // 9600
      newBaud=0x0f;
      break;
    case 0x9B: // <RETURN>
      newBaud=0;
      break;
    default:
      newBaud=0xFF; // Invalid value.
      break;
    }
}

/**
 * main
 */
int main(int argc, char* argv[])
{
  unsigned char err=0;
  
  OS.lmargn=2;
  
  if (_is_cmdline_dos())
    {
      parseopt(argv[1]);
      if ((argc<2) || (newBaud == 0xFF))
	{
	  opts(argv);
	  return(1);
	}
    }
  else
    {
      // DOS 2.0/MYDOS
      print("\x9b");

      while (newBaud==0xFF)
	{
	  print("BAUD RATE, UNLOCK OR \xD2\xC5\xD4\xD5\xD2\xCE? ");
	  get_line(buf,sizeof(buf));
	  parseopt(buf);
	}
    }

  if (newBaud!=0)
    configure(newBaud);

  baud_lock(baudLock);
  
  if (!_is_cmdline_dos())
    {
      print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf,sizeof(buf));
    }
  
  return(err);
}
