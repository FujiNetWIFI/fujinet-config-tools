/**
 * @brief   Show FujiNet Host Slots
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Main Program
 */

#include <stdio.h>
#include <stdint.h>
#include <fujinet-fuji.h>
#include <cc65.h>
#include <apple2.h>

HostSlot hs[8];

void main(void)
{
    unsigned char i=0;

    if (get_ostype() >= APPLE_IIIEM)
    {
        allow_lowercase(true);
    }

    fuji_get_host_slots(hs,8);

    for (i=0;i<8;i++)
    {
        printf("%u:%-32s\n",i+1,hs[i]);
    }
    
    if (doesclrscrafterexit())
    {
        printf("\nPress any key to continue. ");
        getchar();
    }
}
