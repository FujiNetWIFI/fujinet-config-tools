/**
 * FujiNet Tools for CLI
 *
 * fhost - view/change host slots
 *
 * usage:
 *  fhost [hs #] [hostname]
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

const char msg_host_slot[] = "HOST SLOT #";
char buf[80];
static HostSlot hostSlots[8];

/**
 * Wait for keypress
 */
void pause(void)
{
  if (!_is_cmdline_dos())
  {
    print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
    get_line(buf, sizeof(buf));
  }
}

/**
 * Read host slots
 */
void host_read(void)
{
  // Query for host slots
  fn_io_get_host_slots(hostSlots);
  if (OS.dcb.dstats != 1)
  {
    err_sio();
    pause();
    exit(OS.dcb.dstats);
  }
}

/**
 * Mount a host slot
 */
void host_mount(unsigned char c)
{
  if (hostSlots[c][0] != 0x00)
  {
    fn_io_mount_host_slot(c);
    if (OS.dcb.dstats != 1)
    {
      err_sio();
      pause();
      exit(OS.dcb.dstats);
    }
  }
}

/**
 * Write host slots
 */
void host_write(void)
{
  fn_io_put_host_slots(hostSlots);
  if (OS.dcb.dstats != 1)
  {
    err_sio();
    pause();
    exit(OS.dcb.dstats);
  }
}

/**
 * show options
 */
void opts(char *argv[])
{
  print(argv[0]);
  print(" <hs#>,[HOSTNAME]\x9b\x9b");
  print("<hs#> - host slot (1-8)\x9b\x9b");
  print("If hostname is not specified, host slot will be erased.\x9b");
}

/**
 * main
 */
int main(int argc, char *argv[])
{
  unsigned char sa = argv[1][0];
  unsigned char s = sa - 0x30;
  unsigned char i;

  OS.lmargn = 2;

  if (_is_cmdline_dos())
  {
    if (argc < 2)
    {
      opts(argv);
      return (1);
    }

    strcpy(buf, argv[1]);
  }
  else
  {
    print("EDIT HOST SLOT--NUMBER, HOSTNAME?\x9b");
    get_line(buf, sizeof(buf));
  }

  sa = buf[0];
  s = buf[0] - 0x31;
  for (i = 0; i < strlen(buf); i++)
  {
    if (buf[i] == ',')
    {
      i++;
      break;
    }
  }

  // Trim any whitespace
  while (buf[i] == ' ')
    i++;

  if (s < 1 || s > 8)
  {
    print("INVALID SLOT NUMBER.\x9b");
    pause();
    return (1);
  }

  // Read in host and device slots from FujiNet
  host_read();

  // alter host slot
  if (buf[i] == 0x00)
  {
    // Erase host
    memset(hostSlots[s], 0x00, sizeof(hostSlots[s]));
    print(msg_host_slot);
    printc(&sa);
    print("--CLEARED\x9b");
  }
  else
  {
    // Alter host value
    strcpy(hostSlots[s], &buf[i]);
    print(msg_host_slot);
    printc(&sa);
    print("--CHANGED TO:\x9b");
    print(&buf[i]);
    print("\x9b");
  }

  // write the deviceSlots back to FujiNet
  host_write();

  // Mount server
  host_mount(s);

  pause();
  return (0);
}