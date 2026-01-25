/**
 * FujiNet Tools for CLI
 *
 * fmall - Mount all disk slots
 *
 * usage:
 *  fmall
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

unsigned char buf[8];

/**
 * @brief Shown if in ATARI DOS 3
 */
void dos3_clear(void)
{
    OS.rowcrs=9;
    OS.colcrs=2;

	print("\xCD\xEF\xF5\xEE\xF4\xA0\xC1\xEC\xEC\xA0\xC8\xEF\xF3\xF4\xA0\xD3\xEC\xEF\xF4\xF3\x9b\x9b");
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

  print("\x9b");

  print("MOUNTING ALL DEVICE SLOTS...");
  fn_io_mount_all();

  if (OS.dcb.dstats == 1)
  {
    print("OK");
  }
  else
  {
    print("ERROR");
  }

  if (_dos_type == MYDOS)
  {
    print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
    get_line(buf, sizeof(buf));
  }

  return (0);
}
