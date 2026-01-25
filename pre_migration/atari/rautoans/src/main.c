/**
 * FujiNet Tools for CLI
 *
 * rautoans - set auto answer
 *
 * usage:
 *  rautoans <0|1>
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
 * SIO command to set auto ans.
 */
unsigned char auto_ans(unsigned char l)
{
  OS.dcb.ddevic=0x50;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd='O';
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
 * @brief Shown if in ATARI DOS 3
 */
void dos3_clear(void)
{
    OS.rowcrs=9;
    OS.colcrs=2;

    print("\xD3\xE5\xF4\xA0\xD2\xBA\xA0\xC1\xF5\xF4\xEF\xAD\xC1\xEE\xF3\xF7\xE5\xF2\x9b\x9b");
    print("\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c");
}

/**
 * show options
 */
void opts(char* argv[])
{
  print(argv[0]);
  print(" <0|1>\x9b\x9b");
  print(" 0=disable, 1=enable\x9b");
}

/**
 * main
 */
int main(int argc, char* argv[])
{
  unsigned char err=0;
  unsigned char o=0;
  unsigned char i=0;

  if (PEEK(0x718) == 53)
      dos3_clear();

  OS.lmargn=2;

  if (_is_cmdline_dos())
    {
      o=atoi(argv[1]);
    }
  else
    {
      // DOS 2.0/MYDOS
      print("\x9b");

      print("AUTO ANSWER--0=DIS, 1=ENABLE? ");
      get_line(buf,sizeof(buf));
      for (i=0;i<sizeof(buf);i++)
	{
	  if (buf[i]==0x9B)
	    buf[i]=0x00;
	}
    }

  err=auto_ans(o);

  if (_dos_type == MYDOS)
    {
      print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf,sizeof(buf));
    }

  return(err != 1);
}
