/**
 * @brief   Show FujiNet Device Slots
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

DeviceSlot ds[6];
unsigned char i;
char ostype;

char mode(uint8_t mode)
{
    if (mode == 3)
        return 'W';
    else
        return 'R';
}

void print_device_slot(unsigned char i)
{
    if (ostype == APPLE_IIC && i == 4 && ds[i].file[0] == 0x00)
    {
        printf("%u: : :IIC INTERNAL FLOPPY",i);
    }
    else if (!ds[i].file[0])
    {
        printf("%u: : :<EMPTY>\n",i);
    }
    else
    {
        printf("%u:%u:%c:%s\n",i+1,ds[i].hostSlot,mode(ds[i].mode),ds[i].file);
    }
}

void print_smartport_slots(void)
{
    printf("-------------------------SMARTPORT-SLOTS");

    for (i=0;i<4;i++)
    {
        print_device_slot(i);
    }
}

void print_diskii_slots(void)
{
    printf("---------------------------DISK II-SLOTS");

    for (i=4;i<6;i++)
    {
        print_device_slot(i);
    }
}

void main(void)
{
    ostype = get_ostype() & 0xF0;
    
    fuji_get_device_slots(ds,6);

    print_smartport_slots();
    print_diskii_slots();
        
    if (doesclrscrafterexit())
    {
        printf("\nPRESS ANY KEY TO CONTINUE. ");
        getchar();
    }
}
