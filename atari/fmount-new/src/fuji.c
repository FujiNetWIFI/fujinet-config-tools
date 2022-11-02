/**
 * Fuji Device Commands
 */

#include <string.h>
#include <atari.h>
#include "fuji.h"
#include "sio.h"

void set_sio_defaults(void)
{
  OS.dcb.ddevic = 0x70;
  OS.dcb.dunit = 1;
  OS.dcb.dtimlo = 0x0F; // 15 second timeout
}

/**
 * @brief Get Host slots from FujiNet
 * @param Pointer to destination buffer for host slots
 * @return DSTATS from SIO call.
 */
unsigned char fuji_get_host_slots(HostSlots *hs)
{
  // Query for host slots
  set_sio_defaults();
  OS.dcb.dcomnd = 0xF4; // Get host slots
  OS.dcb.dstats = 0x40;
  OS.dcb.dbuf = (unsigned char *)hs;
  OS.dcb.dbyt = sizeof(HostSlots);
  OS.dcb.daux = 0;
  siov();

  return OS.dcb.dstats;
}

/**
 * @brief Mount a host slot
 * @param c The desired host slot
 */
unsigned char fuji_mount_host_slot(unsigned char c)
{
  set_sio_defaults();
  OS.dcb.dcomnd = 0xF9;
  OS.dcb.dstats = 0x00;
  OS.dcb.dbuf = 0;
  OS.dcb.dbyt = 0;
  OS.dcb.daux = c;
  siov();

  return OS.dcb.dstats;
}

/**
 * @brief Open Directory
 * @param hs The selected host slot
 * @param p Pointer to desired path (256 bytes long).
 */
unsigned char fuji_open_directory(unsigned char hs, char *p)
{
    // Open Dir
    set_sio_defaults();
    OS.dcb.dcomnd = 0xF7;
    OS.dcb.dstats = 0x80;
    OS.dcb.dbuf = p;
    OS.dcb.dbyt = 256;
    OS.dcb.daux = hs;
    siov();

    return OS.dcb.dstats;
}

/**
 * @brief Set current directory cursor
 * @param Directory Entry position (0-65535)
 * @return DSTATS
 */
unsigned char fuji_set_directory_position(unsigned short pos)
{
  OS.dcb.dcomnd = 0xE4;
  OS.dcb.dstats = 0x00;
  OS.dcb.dbuf = 0;
  OS.dcb.dbyt = 0;
  OS.dcb.daux = pos;
  siov();

  return OS.dcb.dstats;
}

/**
 * @brief Read next dir entry
 * @param Pointer to directory entry
 * @param desired directory entry length.
 * @return DSTATS
 */
unsigned char fuji_read_directory_entry(char *e, unsigned short len)
{
    memset(e, 0, len);
    e[0] = 0x7f;
    OS.dcb.dcomnd = 0xF6;
    OS.dcb.dstats = 0x40;
    OS.dcb.dbuf = e;
    OS.dcb.dbyt = len;
    OS.dcb.daux = len;
    siov();

    return OS.dcb.dstats;
}

/**
 * Close directory
 * @param hs Host Slot
 * @return DSTATS
 */
unsigned char fuji_close_directory(unsigned char hs)
{
    // Close dir read
    OS.dcb.dcomnd = 0xF5;
    OS.dcb.dstats = 0x00;
    OS.dcb.dbuf = 0;
    OS.dcb.dtimlo = 0x0f;
    OS.dcb.dbyt = 0;
    OS.dcb.daux = hs;
    siov();

    return OS.dcb.dstats;
}

/**
 * Read device slots
 * @param destination buffer for device slots.
 * @return DSTATS
 */
unsigned char fuji_get_device_slots(DeviceSlots* deviceSlots)
{
    // Read Drive Tables
    set_sio_defaults();
    OS.dcb.dcomnd = 0xF2;
    OS.dcb.dstats = 0x40;
    OS.dcb.dbuf = deviceSlots;
    OS.dcb.dbyt = sizeof(DeviceSlots);
    OS.dcb.daux = 0;
    siov();

    return OS.dcb.dstats;
}

/**
 * @brief Set filename for device slot
 * @param slot Device Slot
 * @param filename pointer to filename
 * @return DSTATS
 */
unsigned char fuji_set_device_filename(unsigned char slot, const char* filename)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xE2;
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=(void *)filename;
  OS.dcb.dtimlo=0x0F;
  OS.dcb.dbyt=256;
  OS.dcb.daux1=slot;
  OS.dcb.daux2=0;
  siov();

  return OS.dcb.dstats;
}

/**
 * @brief Write device slots to FujiNet.
 * @param ds Pointer to device slots.
 * @return DSTATS
 */
unsigned char fuji_put_device_slots(DeviceSlots *ds)
{
    set_sio_defaults();
    OS.dcb.dcomnd = 0xF1;
    OS.dcb.dstats = 0x80;
    OS.dcb.dbuf = ds;
    OS.dcb.dbyt = sizeof(DeviceSlots);
    OS.dcb.daux = 0;
    siov();

    return OS.dcb.dstats;
}

/**
 * @brief Mount device slot
 * @param c The device slot to mount
 * @param o The mode to open with.
 * @return DSTATS
 */
unsigned char fuji_mount_device_slot(unsigned char c, unsigned char o)
{
    set_sio_defaults();
    OS.dcb.dcomnd = 0xF8;
    OS.dcb.dstats = 0x00;
    OS.dcb.dbuf = 0;
    OS.dcb.dbyt = 0;
    OS.dcb.dtimlo = 0xFE; // Due to ATX support.
    OS.dcb.daux1 = c;
    OS.dcb.daux2 = o;
    siov();

    return OS.dcb.dstats;
}
