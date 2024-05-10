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

HostSlot hs[8];

void main(void)
{
    unsigned char i=0;

    fuji_get_host_slots(hs,8);

    for (i=0;i<8;i++)
    {
        printf("%u:%-32s\n",i+1,hs[i]);
    }
    
    if (doesclrscrafterexit())
    {
        printf("\nPRESS ANY KEY TO CONTINUE. ");
        getchar();
    }
}
