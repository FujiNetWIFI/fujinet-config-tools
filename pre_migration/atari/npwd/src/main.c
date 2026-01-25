/**
 * Network Testing tools
 *
 * npwd - show N: prefix
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
unsigned char unit;

unsigned char npwd(void)
{
  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd=0x30;
  OS.dcb.dstats=0x40;
  OS.dcb.dbuf=&buf;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=256;
  OS.dcb.daux1=0;
  OS.dcb.daux2=0;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
    }
  return OS.dcb.dstats;
}

/**
 * @brief Shown if in ATARI DOS 3
 */
void dos3_clear(void)
{
    OS.rowcrs=9;
    OS.colcrs=2;

    print("\xD3\xE8\xEF\xF7\xA0\xC3\xF5\xF2\xF2\xE5\xEE\xF4\xA0\xCE\xE5\xF4\xF7\xEF\xF2\xEB\xA0\xC4\xE9\xF2\xE5\xE3\xF4\xEF\xF2\xF9");
    print("\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c");
}

int main(int argc, char* argv[])
{
  unsigned char err=0;

  OS.lmargn=2;

  if (argc<2)
    unit=1;
  else if (argv[1][1]==':')
    unit=1;
  else if ((argv[1][1]>'0') || (argv[1][1]<'9'))
    unit=argv[1][1]-0x30;
  else
    unit=1;

  err=npwd();

  if (buf[0]==0x00)
    print("NO PREFIX SET.\x9b");
  else
    print(buf);

  print("\x9b");

  if (_dos_type == MYDOS || PEEK(0x715) == 0x38)
    {
      print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf,sizeof(buf));
    }

  return err==1 ? 0 : err;
}
