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
#include "conio.h"
#include "err.h"
#include "fn_io.h"

char buf[80];

/**
 * Read Device Slots
 */
void lobby(void)
{
  fn_io_set_boot_mode(2);
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

	print("\xC2\xEF\xEF\xF4\xA0\xD4\xEF\xA0\xC6\xF5\xEA\xE9\xCE\xE5\xF4\xA0\xC7\xE1\xED\xE5\xA0\xCC\xEF\xE2\xE2\xF9\x9b\x9b");
    print("\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c");
}

/**
 * main
 */
int main(void)
{

  OS.lmargn = 2;
  lobby();
  if (PEEK(0x715) == 0x38)
      dos3_clear();

  print("BOOTING TO LOBBY.");
  asm("JMP $E477");

  return (0);
}
