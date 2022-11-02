#include "network.h"

/**
 * @brief Get Network Device Status byte 
 * @param buf Buffer to write
 * @param len Buffer length
 * @param returns DCB status
 */
unsigned char network_statusbyte(void)
{
  DCB *dcb = find_dcb();

  while (1)
    {
      dcb->status = 1; // status
      
      while (dcb->status < 0x80);

      if (dcb->status != 0x9B)
	break;
    }
  
  return dcb->dev_status;
}
