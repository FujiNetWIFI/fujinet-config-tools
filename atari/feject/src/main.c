/**
 * FujiNet Tools for CLI
 *
 * feject - Eject disk in device slot
 *
 * usage:
 *  feject <ds#>
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
static DeviceSlot deviceSlots[8];

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
 * Read Device Slots
 */
void disk_read(void)
{
  fn_io_get_device_slots(deviceSlots);
  if (OS.dcb.dstats != 1)
  {
    err_sio();
    pause();
    exit(OS.dcb.dstats);
  }
}

/**
 * Mount device slot
 */
void disk_umount(unsigned char c)
{
  fn_io_unmount_disk_image(c);
  if (OS.dcb.dstats != 1)
  {
    err_sio();
    pause();
    exit(OS.dcb.dstats);
  }
}

/**
 * Write device slots
 */
void disk_write(void)
{
  fn_io_put_device_slots(deviceSlots);
  if (OS.dcb.dstats != 1)
  {
    err_sio();
    pause();
    exit(OS.dcb.dstats);
  }
}

/**
 * Mount all
 */
void mount_all(void)
{
  fn_io_mount_all();
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
  print(" <ds#>\x9b\x9b");
  print("<ds#> - device slot (1-8)\x9b");
}

/**
 * main
 */
int main(int argc, char *argv[])
{
  unsigned char s = argv[1][0] - 0x30;

  OS.lmargn = 2;

  if (_is_cmdline_dos())
  {
    if (argc < 2)
    {
      opts(argv);
      return (1);
    }
  }
  else
  {
    // DOS 2.0/MYDOS
    print("DEVICE SLOT EJECT--WHICH SLOT?\x9b");
    get_line(buf, sizeof(buf));
    s = buf[0] - 0x30;
  }

  if (s < 1 || s > 8)
  {
    print("INVALID SLOT NUMBER.\x9b");
    pause();
    return (1);
  }

  s -= 1;

  // Read in host and device slots from FujiNet
  disk_read();

  if (deviceSlots[s].hostSlot != 0xFF)
  {
    // unmount disk
    disk_umount(s);

    // mark disk as unmounted in deviceSlots
    deviceSlots[s].hostSlot = 0xFF;
    memset(deviceSlots[s].file, 0, sizeof(deviceSlots[s].file));

    // write the deviceSlots back to FujiNet
    disk_write();

    s += 0x31;
  }
  else
  {
    print("D");
    s += 0x31;
    printc(&s);
    print(": NOT IN USE\x9b");
    pause();
  }

  mount_all();

  return (0);
}