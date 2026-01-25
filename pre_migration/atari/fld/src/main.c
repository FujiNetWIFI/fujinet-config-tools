/**
 * FujiNet Tools for CLI
 *
 * fld - list disk slots
 *
 * usage:
 *  fld
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
static DeviceSlot deviceSlots[8];

/**
 * Read Device Slots
 */
unsigned char disk_read(void)
{
  // Read Drive Tables
  fn_io_get_device_slots(deviceSlots);
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
	OS.rowcrs=9;
	OS.colcrs=2;

    	print("\xCC\xE9\xF3\xF4\xA0\xC6\xF5\xEA\xE9\xCE\xE5\xF4\xA0\xC4\xE5\xF6\xE9\xE3\xE5\xA0\xD3\xEC\xEF\xF4\xF3\x9b\x9b");
    print("\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c");
}

/**
 * main
 */
int main(void)
{
  unsigned char i = 0;
  unsigned char err = 0;

  OS.lmargn = 2;

  if (PEEK(0x715) == 0x38)
      dos3_clear();

  // Read in host and device slots from FujiNet
  err = disk_read();

  if (err == 1)
  {
    print("\x9b");

    for (i = 0; i < 8; i++)
    {
      unsigned char n = i + 0x31;
      unsigned char hs = deviceSlots[i].hostSlot + 0x31;
      unsigned char m = (deviceSlots[i].mode == 0x02 ? 'W' : 'R');

      if (deviceSlots[i].hostSlot != 0xFF)
      {
        print("D");
        printc(&n);
        print(": ");
        print("(");
        printc(&hs);
        print(") ");
        print("(");
        printc(&m);
        print(") ");
        print(deviceSlots[i].file);
        print("\x9b");
      }
      else
      {
        print("D");
        printc(&n);
        print(": ");
        print("Empty\x9b");
      }
    }
  }

  print("\x9b");

  if (_dos_type == MYDOS || PEEK(0x715) == 0x38)
  {
    print("PRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
    get_line(buf, sizeof(buf));
  }

  return err == 1 ? 0 : err;
}
