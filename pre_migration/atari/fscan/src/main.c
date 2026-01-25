/**
 * FujiNet Tools for CLI
 *
 * fscan - scan and return list of wireless networks
 *
 * usage:
 *  fscan
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

char buf[40];
static SSIDInfo ssidInfo;

unsigned char num_networks;

/**
 * Return number of networks
 */
unsigned char scan(void)
{
  num_networks = fn_io_scan_for_networks();
  if (OS.dcb.dstats != 0x01)
  {
    err_sio();
  }
  return OS.dcb.dstats;
}

/**
 * Return Network entry from last scan
 */
unsigned char scan_result(unsigned char n)
{
  fn_io_get_scan_result(n, &ssidInfo);
  if (OS.dcb.dstats != 0x01)
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

	print("\xD3\xE3\xE1\xEE\xA0\xE6\xEF\xF2\xA0\xD7\xE9\xC6\xE9\xA0\xCE\xE5\xF4\xF7\xEF\xF2\xEB\xF3\x9b\x9b");
    print("\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c");
}

/**
 * main
 */
int main(void)
{
  unsigned char i = 0;
  unsigned char err = 0;

  if (PEEK(0x715) == 0x38)
      dos3_clear();

  OS.lmargn = 2;

  print("\x9b");

  OS.lmargn = 2;

  print("Scanning...\x9b");
  err = scan();

  if (err == 1)
  {
    for (i = 0; i < num_networks; i++)
    {
      err = scan_result(i);
      if (err != 1)
        break;
      print("* ");
      print(ssidInfo.ssid);
      print("\x9b");
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
