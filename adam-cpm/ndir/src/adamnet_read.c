#include "network.h"

#define READ_MAX_LEN 1024

/**
 * @brief Read from Network device
 * @param buf Buffer to read to (must have enough room, 1024 bytes max)
 * @param len Buffer length
 * @param returns DCB status
 */
unsigned char adamnet_read(char *buf, unsigned short len)
{
  DCB *dcb = find_dcb();

  while (1)
    {
      dcb->len = READ_MAX_LEN;
      dcb->buf = buf;
      dcb->status = 4; // read
      
      while (dcb->status < 0x80);

      if (dcb->status != 0x9B)
	break;
    }
  
  return dcb->status;
}
