/**
 * FujiNet Tools for CLI
 *
 * fnet - set Network SSID/Password
 *
 * usage:
 *  fnet <SSID> [PASSWORD]
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

unsigned char buf[128];
static NetConfig netConfig;

/**
 * Wait for keypress
 */
void pause(void) {
  if (!_is_cmdline_dos()) {
    print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
    get_line(buf, sizeof(buf));
  }
}

/**
 * main
 */
int main(int argc, char *argv[]) {
  unsigned char wifiStatus = 0;
  char *tokens[2];
  unsigned char i;

  OS.lmargn = 2;

  if (_is_cmdline_dos()) {
    if (argc < 2) {
      print(argv[0]);
      print(" <SSID>,<PASSWORD>\x9b");
      exit(1);
    }

    for (i = 1; i < argc; i++) {
      strcat(buf, argv[i]);
      if (i < argc - 1)
        strcat(buf, " ");
    }
  } else {
    print("\x9b");
    print("NETWORK--SSID, PASSWORD?\x9b");
    get_line(buf, sizeof(buf));
  }

  tokens[0] = strtok(buf, ",");
  tokens[1] = strtok(NULL, ",");

  strcpy(netConfig.ssid, tokens[0]);
  strcpy(netConfig.password, tokens[1]);

  if (tokens[0] == NULL) {
    print("NETWORK NAME REQUIRED\x9b");
    pause();
    return (1);
  }

  print("CONNECTING--");
  print(netConfig.ssid);
  print("...\x9b");

  fn_io_set_ssid(&netConfig);
  if (OS.dcb.dstats == 1) {
    // Wait for connection
    OS.rtclok[0] = OS.rtclok[1] = OS.rtclok[2] = 0;

    while (OS.rtclok[1] < 1) {
      wifiStatus = fn_io_get_wifi_status();
      if (wifiStatus == 0x03) // IW_CONNECTED
      {
        print("OK!\x9b");
        return 0;
      }
    }

    print("\x9bNOT CONNECTED.\x9b");
  } else {
    err_sio();
  }

  OS.soundr = 1;

  pause();
  return OS.dcb.dstats != 1 ? OS.dcb.dstats : wifiStatus;
}