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
 * main
 */
int main(void)
{
  unsigned char err = 0;

  OS.lmargn = 2;

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

  if (!_is_cmdline_dos())
  {
    print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
    get_line(buf, sizeof(buf));
  }

  return err == 1 ? 0 : err;
}
