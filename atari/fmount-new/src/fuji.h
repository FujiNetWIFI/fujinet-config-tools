/**
 * Fuji Device Commands
 */

#ifndef FUJI_H
#define FUJI_H

#define FILE_MAXLEN 36

typedef unsigned char HostSlots[8][32]; 

typedef struct
{
  unsigned char hostSlot;
  unsigned char mode;
  unsigned char file[FILE_MAXLEN];
} DeviceSlots[8];

/**
 * @brief Get Host slots from FujiNet
 * @param Pointer to destination buffer for host slots
 * @return DSTATS from SIO call.
 */
unsigned char fuji_get_host_slots(HostSlots *hs);

/**
 * @brief Mount a host slot
 * @param c The desired host slot
 * @return DSTATS
 */
unsigned char fuji_mount_host_slot(unsigned char c);

/**
 * @brief Open Directory
 * @param hs The selected host slot
 * @param p Pointer to desired path (256 bytes long).
 * @return DSTATS
 */
unsigned char fuji_open_directory(unsigned char hs, char *p);

/**
 * @brief Set current directory cursor
 * @param Directory Entry position (0-65535)
 * @return DSTATS
 */
unsigned char fuji_set_directory_position(unsigned short pos);

/**
 * @brief Read next dir entry
 * @param Pointer to directory entry
 * @param desired directory entry length.
 * @return DSTATS
 */
unsigned char fuji_read_directory_entry(char *e, unsigned short len);

/**
 * Close directory
 * @param hs Host Slot
 * @return DSTATS
 */
unsigned char fuji_close_directory(unsigned char hs);

/**
 * Read device slots
 * @param destination buffer for device slots.
 * @return DSTATS
 */
unsigned char fuji_get_device_slots(DeviceSlots* deviceSlots);

/**
 * @brief Set filename for device slot
 * @param slot Device Slot
 * @param filename pointer to filename
 * @return DSTATS
 */
unsigned char fuji_set_device_filename(unsigned char slot, const char* filename);

/**
 * @brief Write device slots to FujiNet.
 * @param ds Pointer to device slots.
 * @return DSTATS
 */
unsigned char fuji_put_device_slots(DeviceSlots *ds);

/**
 * @brief Mount device slot
 * @param c The device slot to mount
 * @param o The mode to open with.
 * @return DSTATS
 */
unsigned char fuji_mount_device_slot(unsigned char c, unsigned char o);

#endif /* FUJI_H */
