/**
 * FujiNet Tools for CLI
 *
 * freset - reset fujinet.
 *
 * usage:
 *  freset
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
#include "conio.h"
#include "err.h"
#include "fn_io.h"

char buf[80];

/**
 * Read Device Slots
 */
void reset(void)
{
  fn_io_reset();
  if (OS.dcb.dstats != 1)
  {
    err_sio();
    if (_dos_type == MYDOS)
    {
      print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf, sizeof(buf));
    }
    exit(OS.dcb.dstats);
  }
}

/**
 * @brief Shown if in ATARI DOS 3
 */
void dos3_clear(void)
{
    OS.rowcrs=9;
    OS.colcrs=2;

	print("\xC2\xEF\xEF\xF4\xA0\xC6\xF5\xEA\xE9\xCE\xE5\xF4\xA0\xC9\xEE\xF4\xEF\xA0\xC3\xCF\xCE\xC6\xC9\xC7\x9b\x9b");
    print("\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c");
}

/**
 * main
 */
int main(void)
{

  OS.lmargn = 2;

  if (PEEK(0x715) == 0x38)
      dos3_clear();

  reset();
  OS.rtclok[0] = OS.rtclok[1] = OS.rtclok[2] = 0;
  print("FUJINET RESET. REBOOTING...");
  while (OS.rtclok[1] < 2) { }
  asm("JMP $E477");

  return (0);
}
