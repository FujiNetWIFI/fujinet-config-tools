/**
 * FujiNet Tools for CLI
 *
 * rbaudlok - lock baud rate.
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
#include <peekpoke.h>

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
  OS.dcb.daux2=0;

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
  // param was baud rate
  switch (o[0])
    {
    case '0': // No baud change
      break;
    case '3': // 300
      newBaud=0x08;
      break;
    case '6': // 600
      newBaud=0x09;
      break;
    case '1': // 1200 or 19200
      newBaud=(o[1]==2 ? 0x0a : 0x0f);
      break;
    case '2': // 2400
      newBaud=0x0c;
      break;
    case '4': // 4800
      newBaud=0x0d;
      break;
    case '9': // 9600
      newBaud=0x0f;
      break;
    case 'U': // UNLOCK
      newBaud=baudLock=0;
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
 * @brief Shown if in ATARI DOS 3
 */
void dos3_clear(void)
{
    OS.rowcrs=9;
    OS.colcrs=2;

    print("\xCC\xEF\xE3\xEB\xA0\xD2\xBA\xA0\xC2\xE1\xF5\xE4\xA0\xD2\xE1\xF4\xE5\x9B\x9B");
    print("\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c");
}

/**
 * main
 */
int main(int argc, char* argv[])
{
  unsigned char err=0;

  if (PEEK(0x715) == 0x38)
      dos3_clear();

  OS.lmargn=2;

  if (_is_cmdline_dos())
    {
      parseopt(argv[1]);
      if (newBaud == 0xFF)
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

  if (_dos_type == MYDOS)
    {
      print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf,sizeof(buf));
    }

  return(err);
}
