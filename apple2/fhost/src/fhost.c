/**
 * @brief   Set/Clear a Host Slot
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
#include <apple2.h>
#include "read_line.h"

unsigned char selected_slot=0;
HostSlot hs[8];
char new_host_name[32];

void fhost(unsigned char slot, char *s)
{
    slot--;
    
    fuji_get_host_slots(hs,8);

    memcpy(&hs[slot],s,32);

    fuji_put_host_slots(hs,8);

    slot++;
    
    printf("\n\nHost slot %u set to: %s\n\n",slot,s);
}

void main(void)
{
    if (get_ostype() >= APPLE_IIE)
    {
        allow_lowercase(true);
    }

    printf("\nChange host slot--Which slot? (1-8)\n");
    scanf("%u",&selected_slot);
    
    if (!selected_slot || selected_slot > 8)
    {
        return;
    }

    printf("\nNew host name, or RETURN to clear\n");

    read_line(new_host_name,sizeof(new_host_name));

    fhost(selected_slot,new_host_name);

    if (doesclrscrafterexit())
    {
        printf("\nPress any key to continue. ");
        getchar();
    }
}
