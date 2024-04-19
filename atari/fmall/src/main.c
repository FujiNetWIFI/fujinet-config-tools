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
 * main
 */
int main(void)
{

  OS.lmargn = 2;

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

  if (!_is_cmdline_dos())
  {
    print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
    get_line(buf, sizeof(buf));
  }

  return (0);
}