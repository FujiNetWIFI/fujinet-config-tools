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
#include "read_line.h"
#include "text.h"

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
    
    printf("\n\nHOST SLOT %u SET TO: %s\n\n",slot,s);
}

void main(void)
{
    printf("\nCHANGE HOST SLOT--WHICH SLOT? (1-8)\n");
    scanf("%u",&selected_slot);
    
    if (!selected_slot || selected_slot > 8)
    {
        return;
    }

    printf("\nNEW HOST NAME, OR RETURN TO CLEAR\n");

    read_line(new_host_name,sizeof(new_host_name));

    fhost(selected_slot,new_host_name);

    if (doesclrscrafterexit())
    {
        printf("\nPRESS ANY KEY TO CONTINUE. ");
        getchar();
    }
}
