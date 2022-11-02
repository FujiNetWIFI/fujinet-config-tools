#include <stdlib.h>
#include "network.h"

/**
 * @brief Find FujiNet DCB
 * @return Pointer to PCB
 */
DCB* find_dcb(void)
{
  char num_dcbs =  (*(unsigned char*) 0xFEC3);
  DCB *current_dcb = (*(DCB *)0xFEC4);
  char i;

  for (i=0;i<num_dcbs;i++)
    {
      if ((current_dcb[i].dev & 0x0F) == 0x0F) // 0x0F = Fuji Device
	return current_dcb[i];
    }

  // Device not found, return NULL.
  return NULL;
}
