/**
 * FujiNet Tools for CLI
 *
 * flobby - Reset to game lobby.
 *
 * usage:
 *  flobby
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
#include <peekpoke.h>
#include "sio.h"
#include "conio.h"
#include "err.h"

char buf[80];

/**
 * Read Device Slots
 */
void lobby(void)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xD6;
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=NULL;
  OS.dcb.dtimlo=0x0f;
  OS.dcb.dbyt=0;
  OS.dcb.daux=2;
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

/**
 * main
 */
int main(void)
{

  OS.lmargn=2;
  lobby();
  print("BOOTING TO LOBBY.");
  asm("JMP $E477");
  
  return(0);
}
