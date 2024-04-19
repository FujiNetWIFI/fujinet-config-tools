/**
 * FujiNet Tools for CLI
 *
 * fnew - make new ATR disk on host
 *
 * usage:
 *  fnew <ds#> <hs#> <sect> <sectsize> <fname>
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
static HostSlot hostSlots[8];
static DeviceSlot deviceSlots[8];
static NewDisk newDisk;

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
 * Create disk in slot
 */
void disk_create(unsigned short ns, unsigned short ss, unsigned char hs, unsigned char ds, char *fname)
{
  newDisk.numSectors = ns;
  newDisk.sectorSize = ss;
  newDisk.hostSlot = hs;
  newDisk.deviceSlot = ds;
  strcpy(newDisk.filename, fname);

  // Create new disk
  fn_io_create_new(&newDisk);
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
 * Mount device slot
 */
void disk_mount(unsigned char c, unsigned char o)
{
  fn_io_mount_disk_image(c, o);
  if (OS.dcb.dstats != 1)
  {
    err_sio();
    pause();
    exit(OS.dcb.dstats);
  }
}

/**
 * Set filename for device slot
 */
void set_filename(char *filename, unsigned char device_slot, unsigned char host_slot, unsigned char mode)
{
  fn_io_set_device_filename(mode, host_slot, device_slot, filename);
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
  print(" <DS#>,[HS#],[T],[FNAME]\x9b\x9b"
        "<DS#>   - device slot (1-8)\x9b"
        "[HS#]   - host slot (1-8)\x9b"
        "[T]     - type, one of:\x9b"
        "\t\t1: 90K    (SS/SD)\x9b"
        "\t\t2: 140K   (SS/ED)\x9b"
        "\t\t3: 180K   (SS/DD)\x9b"
        "\t\t4: 360K   (DS/DD)\x9b"
        "\t\t5: 720K   (DS/QD)\x9b"
        "\t\t6: 1440K  (DS/HD), or...\x9b"
        "\t\t1-65535:128|256 (Custom)\x9b"
        "[FNAME] - Image Filename\x9b");
}

/**
 * main
 */
int main(int argc, char *argv[])
{
  unsigned char ds, hs, dsa, hsa;
  unsigned short ns, ss;
  char *tokens[4];
  unsigned char i;
  char *nst;
  char *sst;

  OS.lmargn = 2;

  if (_is_cmdline_dos())
  {
    memset(buf, 0, sizeof(buf));

    if (argc < 2)
    {
      opts(argv);
      return (1);
    }
    for (i = 1; i < argc; i++)
    {
      strcat(buf, argv[i]);
      if (i < argc - 1)
        strcat(buf, " ");
    }
  }
  else
  {
    // DOS 2.0
    // "                                     "
    print("DISK TYPES: 1--90K, 2--140K, 3--180K \x9b"
          "4--360K, 5--720K, 6--1440K, OR\x9b"
          "# of SECTORS:SECTOR SIZE"
          "\x9b"
          "NEW--DEVICE, HOST SLOT, TYPE, NAME?\x9b");

    get_line(buf, 128);
  }

  // Tokenize and trim any whitespace after each ,
  tokens[0] = strtok(buf, ",");
  for (i = 1; i < 4; i++)
  {
    tokens[i] = strtok(NULL, ",");
    while (*tokens[i] == ' ')
      tokens[i]++;
  }

  if (tokens[0] == NULL ||
      tokens[1] == NULL ||
      tokens[2] == NULL ||
      tokens[3] == NULL)
  {
    print("ALL ARGUMENTS REQUIRED");
    pause();
    return (1);
  }

  hsa = *tokens[1];
  hs = hsa - 0x30;

  dsa = *tokens[0];
  ds = dsa - 0x30;

  if (ds < 1 || ds > 8)
  {
    print("INVALID DRIVE SLOT NUMBER.\x9b");
    pause();
    return (1);
  }

  if (hs < 1 || hs > 8)
  {
    print("INVALID HOST SLOT NUMBER.\x9b");
    pause();
    return (1);
  }

  hs -= 1;
  ds -= 1;

  // Read in existing data from FujiNet
  host_read();
  disk_read();

  // Mount desired host
  host_mount(hs);

  // Set desired host slot and mode

  deviceSlots[ds].mode = 2; // R/W
  deviceSlots[ds].hostSlot = hs;
  set_filename(tokens[3], ds, hs, 2);

  // Write out disk slot
  disk_write();

  // Determine disk type
  nst = strtok(tokens[2], ":");
  sst = strtok(NULL, ":");

  if (sst != NULL)
  {
    ns = atoi(nst);
    ss = atoi(sst);
  }
  else
  {
    switch (tokens[2][0])
    {
    case '1':
      ns = 720;
      ss = 128;
      break;
    case '2':
      ns = 1040;
      ss = 128;
      break;
    case '3':
      ns = 720;
      ss = 256;
      break;
    case '4':
      ns = 1440;
      ss = 256;
      break;
    case '5':
      ns = 720;
      ss = 256;
      break;
    case '6':
      ns = 5760;
      ss = 256;
      break;
    }
  }
  // Create the disk
  print("\x9b");
  print("CREATING DISK\x9b");

  disk_create(ns, ss, hs, ds, tokens[3]);
  /* disk_write(); */
  disk_mount(ds, 2);

  print("D");
  printc(&dsa);
  print(": ");
  print("(");
  printc(&hsa);
  print(") ");
  print("(");
  print("W");
  print(") ");
  print(tokens[3]);
  print("\x9b");

  pause();
  return 0;
}
