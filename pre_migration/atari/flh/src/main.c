/**
 * FujiNet Tools for CLI
 *
 * flh - list host slots
 *
 * usage:
 *  flh
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

unsigned char buf[40];
static HostSlot hostSlots[8];

/**
 * Read host slots
 */
unsigned char host_read(void)
{
  // Query for host slots
  fn_io_get_host_slots(hostSlots);
  if (OS.dcb.dstats != 1)
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
    print("\x1c\x1c\x1c\x1c\x1c\x1c\x1c\x1c\x1c\x1c");
    print("\xCC\xE9\xF3\xF4\xA0\xC6\xF5\xEA\xE9\xCE\xE5\xF4\xA0\xC8\xEF\xF3\xF4\xA0\xD3\xEC\xEF\xF4\xF3");
    print("\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c");
}

/**
 * main
 */
int main(void)
{
  unsigned char i = 0;

  if (PEEK(0x718) == 53)
      dos3_clear();

  OS.lmargn = 2;

  // Read in host and device slots from FujiNet
  host_read();

  print("\x9b");

  for (i = 0; i < 8; i++)
  {
    unsigned char n = i + 0x31;

    if (hostSlots[i][0] != 0x00)
    {
      printc(&n);
      print(": ");
      print(hostSlots[i]);
      print("\x9b");
    }
    else
    {
      printc(&n);
      print(": ");
      print("Empty\x9b");
    }
  }

  print("\x9b");

  if (_dos_type == MYDOS || PEEK(0x718) == 53)
  {
    print("PRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
    get_line(buf, sizeof(buf));
  }

  return 0;
}
