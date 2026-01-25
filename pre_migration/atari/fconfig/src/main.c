/**
 * FujiNet Tools for CLI
 *
 * fconfig - Show FujiNet Configuration
 *
 * usage:
 *  fconfig
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
#include "version.h"
#include "fn_io.h"

unsigned char buf[255];
static AdapterConfigExtended adapterConfig;

/**
 * Read Device Slots
 */
unsigned char adapter_config(void)
{
  fn_io_get_adapter_config_extended(&adapterConfig);
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

    print("\xC6\xF5\xEA\xE9\xCE\xE5\xF4\xA0\xCE\xE5\xF4\xF7\xEF\xF2\xEB\xA0\xC3\xEF\xEE\xE6\xE9\xE7\xF5\xF2\xE1\xF4\xE9\xEF\xEE");
    print("\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c");
}

/**
 * main
 */
int main(void)
{
  unsigned char err = 0;

  OS.lmargn = 2;

  if (PEEK(0x715) == 0x38)
      dos3_clear();

  // Read adapter config
  err = adapter_config();

  if (err == 1)
  {
    print("\x9b");

    print("           SSID: ");
    print(adapterConfig.ssid);
    print("\x9b");

    print("       Hostname: ");
    print(adapterConfig.hostname);
    print("\x9b");

    print("     IP Address: ");
    print(adapterConfig.sLocalIP);
    print("\x9b");

    print("Gateway Address: ");
    print(adapterConfig.sGateway);
    print("\x9b");

    print("    DNS Address: ");
    print(adapterConfig.sDnsIP);
    print("\x9b");

    print("        Netmask: ");
    print(adapterConfig.sNetmask);
    print("\x9b");

    print("    MAC Address: ");
    print(adapterConfig.sMacAddress);
    print("\x9b");

    print("          BSSID: ");
    print(adapterConfig.sBssid);
    print("\x9b");

    print("FCONFIG Version: ");
    print(FCONFIG_VERSION_FULL);
    print("\x9b");

    print("  Fuji Firmware: ");
    print(adapterConfig.fn_version);
    print("\x9b");
  }

  if (_dos_type == MYDOS)
  {
    print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
    get_line(buf, sizeof(buf));
  }

  return err == 1 ? 0 : err;
}
