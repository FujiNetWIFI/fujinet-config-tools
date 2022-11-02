/**
 * List Host Slots
 */

#include <fujicpm.h>
#include <stdio.h>

const char *mode(char m)
{
  return m == 0x02 ? "R/W" : "R/O";
}

void main(void)
{
  DeviceSlot ds[8];

  printf("\nDEVICE SLOTS\n\n");
  fuji_get_device_slots(ds);

  for (char i=0;i<4;i++)
  {
    printf("[%d]",i+1);
    if (ds[i].hostSlot == 0xFF)
      printf("--Empty--");
    else
      printf("HS:%d M: %s F: %s",ds[i].hostSlot,mode(ds[i].mode),ds[i].file);

    printf("\n");
  }
}
