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
 * main
 */
int main(void)
{
  unsigned char i = 0;
  unsigned char err = 0;

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

  if (!_is_cmdline_dos())
  {
    print("PRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
    get_line(buf, sizeof(buf));
  }

  return err == 1 ? 0 : err;
}
