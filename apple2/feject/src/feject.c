/**
 * @brief   Eject a FujiNet Device Slot
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Main Program
 */

#include <stdio.h>
#include <stdint.h>
#include <fujinet-fuji.h>
#include <cc65.h>
#include <string.h>

unsigned char selected_slot=0;
DeviceSlot ds[6];

void feject(unsigned char slot)
{

    slot--;
    
    fuji_get_device_slots(ds,6);

    fuji_unmount_disk_image(slot);

    ds[slot].hostSlot=0xFF;
    memset(ds[slot].file, 0, sizeof(ds[slot].file));

    fuji_put_device_slots(ds,6);

    slot++;

    printf("\nDEVICE SLOT %u EJECTED.\n",slot);    
}

void main(void)
{
    printf("\nEJECT DEVICE SLOT--WHICH SLOT? (1-4)\n");
    scanf("%u",&selected_slot);
    
    if (!selected_slot || selected_slot > 4)
    {
        return;
    }

    feject(selected_slot);

    if (doesclrscrafterexit())
    {
        printf("\nPRESS ANY KEY TO CONTINUE. ");
        getchar();
    }
}
