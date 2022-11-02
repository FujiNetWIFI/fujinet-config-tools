#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <fujicpm.h>

void eject(unsigned char slot)
{
  DeviceSlot ds[8];

  fuji_get_device_slots(ds);

  fuji_unmount_device_slot(slot);

  ds[slot].hostSlot=0xFF;
  ds[slot].mode=0;
  memset(ds[slot].file,0,36);

  fuji_put_device_slots(ds);
  
}

void show_device_slots(void)
{
  DeviceSlot ds[8];

  fuji_get_device_slots(ds);

  cprintf("\nDEVICE SLOTS\n\n");
  
  for (char i=0;i<4;i++)
    {
      cprintf("[%d]",i+1);
      if (ds[i].file == 0x00)
	cprintf("--Empty--\n");
      else
	cprintf("%s\n",ds[i].file);
    }
}

unsigned char ask(void)
{
  char k=0;
  
  show_device_slots();

  while ((k < 0x31) || (k > 0x34))
    {
      cprintf("\nWHICH SLOT TO EJECT? ");
      k=cgetc();
      cprintf("%c\n",k);

      if (k == 0x03)
	{
	  cprintf("ABORTING.\n");
	  exit(0);
	}
    }
  
  return k-0x30;
}

int main(int argc, char *argv[])
{
  unsigned char slot;
  
  if (argc < 2)
    slot=ask();
  else
    slot=argv[1][0] -= 0x30;

  if (slot < 1 || slot > 4)
    {
      cprintf("INVALID SLOT\n");
      return 1;
    }

  slot--;
  
  eject(slot);

  return 0;
}
